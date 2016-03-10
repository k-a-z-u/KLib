#ifndef K_DATA_KDTREE_KDTREE_H
#define K_DATA_KDTREE_KDTREE_H

#include <vector>
#include <algorithm>

#include "KDTreeData.h"
#include "KDTreeSplit.h"
#include "KDTreeHelper.h"

#include "../../Assertions.h"

namespace K {

	/** KD-Tree configuration */
	template <typename _Scalar, typename _DataSource, int _Dimensions, typename _Splitter> struct KDTreeConfig {
		using Scalar = _Scalar;
		using DataSource = _DataSource;
		using Splitter = _Splitter;
		static constexpr int Dimensions = _Dimensions;
	};

	/**
	 * a very basic KD-Tree to sort user-Elements of a given Dimension
	 *
	 * Example:
	 *		Scalar:		float
	 *		DataSource:	PointCloud
	 *		Dimension:	3
	 */
	template <typename Config> class KDTree {

	public:
		using DataSource = typename Config::DataSource;
		using Scalar = typename Config::Scalar;
		using Splitter = typename Config::Splitter;
		static constexpr int Dimensions = Config::Dimensions;

		using _KDTreeElem = KDTreeElem<Scalar>;
		using _KDTreeNode = KDTreeNode<Scalar>;
		using _KDTreeLeaf = KDTreeLeaf<Scalar>;

	private:
		Splitter splitter;



	private:

		/** the root node */
		_KDTreeElem* root;

		/** the maximum depth to use when building the tree */
		const int maxDepth;

		/** the maximum number of elements per leaf */
		const KDIdx maxPerLeaf;

		/** the user's data source */
		const DataSource* source;

	public:

		/** ctor */
		KDTree(const int maxDepth = 10, const int maxPerLeaf = 32) : root(nullptr), maxDepth(maxDepth), maxPerLeaf(maxPerLeaf), source(nullptr) {
			;
		}

		/** dtor */
		~KDTree() {
			cleanup();
		}


		/** no copy */
		KDTree(const KDTree&) = delete;

		/** no assign */
		void operator = (const KDTree&) = delete;


		/**
		 * set the underlying user data source to build the index for.
		 * this data-structure must provide some access methods to fetch
		 * elements / distances by index.
		 * using this method will destroy the current tree index!
		 */
		void setDataSource(const DataSource* ds) {

			this->source = ds;

			// delete previous data
			cleanup();

			// start with an empty root
			root = new _KDTreeLeaf(nullptr);


		}


		/**
		 * add all elements [0:cnt-1] from the DataSource to the tree.
		 * using this method will destroy the current tree index!
		 */
		void addAll(const KDIdx cnt) {

			// sanity checks
			_assertNotNull(source, "call setDataSource() first!");

			// any previous data?
			cleanup();

			// add all element-indicies to the first leaf
			_KDTreeLeaf* leaf = new _KDTreeLeaf(nullptr);
			leaf->entries.resize(cnt);
			for (KDIdx i = 0; i < cnt; ++i) {leaf->entries[i] = i;}

			// split this leaf (recursively) until it is small enough
			root = leaf;
			root = (_KDTreeNode*) splitIfNeeded(leaf, 0, 0);

		}

		/** add a new entry by its id/idx within the DataSource */
		void addByID(const KDIdx idx, const bool balance = false) {

			// sanity checks
			_assertNotNull(source, "call setDataSource() first!");

			// find the leaf for the element
			_KDTreeLeaf* leaf = (_KDTreeLeaf*) getLeafFor(idx);

			// add it
			leaf->entries.add(idx);

			// split the leaf (if needed)
			const int axis = (!leaf->parent) ? (0) : (nextAxis(leaf->parent->asNode()->splitAxis));
			_KDTreeElem* elem = (_KDTreeNode*) splitIfNeeded(leaf, axis, 0);

			// changed? (leaf is deleted by now!)
			if (elem != leaf) {
				if (leaf == root)	{root = elem;}									// new root
				if (leaf->parent)	{leaf->getParent()->switchChild(leaf, elem);}	// update association
			}

			// perform balancing?
			if (balance) {checkAndBalance(elem);}

		}

		/** delete one entry by its id/idx within the DataSource */
		void deleteByID(const KDIdx idx, const bool balance = true) {

			// sanity checks
			_assertNotNull(source, "call setDataSource() first!");

			// find the leaf for the element
			_KDTreeLeaf* leaf = (_KDTreeLeaf*) getLeafFor(idx);

			// remove it
			leaf->entries.remove(idx);

			// rebalance?
			if (balance) {checkAndBalance(leaf);}

		}

		/** get the number of entries below the given element (top-down) */
		int getNumElementsBelow(const _KDTreeElem* elem) const {

			// empty node reached
			if (!elem) {return 0;}

			// leaf reached?
			if (elem->isLeaf()) {return (KDIdx) elem->asLeaf()->entries.size();}

			// node reached -> recursion
			return getNumElementsBelow(elem->asNode()->left) + getNumElementsBelow(elem->asNode()->right);

		}

		/** get a list of all entries below the given element (top-down) */
		void getElementsBelow(const _KDTreeElem* elem, std::vector<KDIdx>& elems) const {

			// empty node reached
			if (!elem) {return;}

			// leaf reached?
			if (elem->isLeaf()) {

				const KDIdx* data = elem->asLeaf()->entries.data();
				const KDIdx  size = elem->asLeaf()->entries.size();
				elems.insert(elems.end(), data, data + size);

			} else {

				// node reached -> recursion
				getElementsBelow(elem->asNode()->left, elems);
				getElementsBelow(elem->asNode()->right, elems);

			}

		}

		/** get the distance between the given coordinates and the Element provided by idx */
		inline Scalar getDistance(const KDIdx idx, const Scalar values[Dimensions]) const {
			return source->kdGetDistance(idx, values);
		}

		/** get the value for one axis of the idx-th element */
		inline Scalar getValue(const KDIdx idx, const int axis) const {
			return source->kdGetValue(idx, axis);
		}

		/**
		 * get the left-right ratio for this node.
		 * > 1: more on the right side
		 * < 1: more on the left side
		 */
		KDRatio getRatio(const _KDTreeElem* elem) const {

			if (elem->isLeaf()) {
				throw "error";
			}

			const KDIdx left = getNumElementsBelow(elem->asNode()->left);
			const KDIdx right = getNumElementsBelow(elem->asNode()->right);

			return KDRatio(left, right);

		}


		/** get the tree's root node */
		const _KDTreeElem* getRoot() const {
			_assertNotNull(root, "data-structure error. root is null");
			return root;
		}



		/** get the leaf the element with the given idx/id belongs to */
		const _KDTreeLeaf* getLeafFor(const KDIdx idx) const {
			float values[Dimensions];
			for (int ax = 0; ax < Dimensions; ++ax) {
				values[ax] = source->kdGetValue(idx, ax);
			}
			return getLeafFor(values);
		}

		/** get the leaf the given coordinates belongs to */
		const _KDTreeLeaf* getLeafFor(const std::initializer_list<float> lst) const {
			return getLeafFor(lst.begin());
		}

		/** get the leaf the given coordinates belongs to */
		const _KDTreeLeaf* getLeafFor(const Scalar values[Dimensions]) const {

			// start at the root
			const _KDTreeElem* cur = getRoot();

			// top-down
			while (true) {

				// reached a leaf?
				if (cur->isLeaf()) {
					_assertTrue(cur->parent || root==cur, "data-structure error. leaf without parent");
					return cur->asLeaf();
				}

				// left or right?
				const _KDTreeNode* node = cur->asNode();
				if (KDTreeHelper::leftOf(node, values)) {
					cur = node->left;
				} else {
					cur = node->right;
				}

				// sanity check
				_assertNotNull(cur, "data-structure error. no leaf found");

			}

		}

	private:



		/**
		 * if the given leaf does not conform with the configured settings (max number of elements)
		 * split it into a new node with two leafs (left/right)
		 */
		_KDTreeElem* splitIfNeeded(_KDTreeLeaf* leaf, const int axis, const int depth) {

			// sanity check
			_assertTrue(leaf == root || leaf->parent, "non-root without parent");

			// reached max-depth? -> do not split any further
			if (depth > maxDepth) {return leaf;}

			// nothing to split?
			if ((KDIdx)leaf->entries.size() <= maxPerLeaf) {return leaf;}

			// get the splitting center for all leaf-values of the given dimension
			const Scalar center = splitter.getCenter(source, leaf->entries, axis, Config());

			// new data-structure (new node + 2 leafs)
			_KDTreeNode* newNode = new _KDTreeNode(center, axis, leaf->parent);
			_KDTreeLeaf* newLeft = new _KDTreeLeaf(newNode);
			_KDTreeLeaf* newRight = new _KDTreeLeaf(newNode);
			newNode->attach(newLeft, newRight);

			// reserve space (slightly faster)
			newLeft->entries.reserve( leaf->entries.size() * 6 / 10 );
			newRight->entries.reserve( leaf->entries.size() * 6 / 10 );

			// split the old leaf into the 2 new ones
			for (int i = 0; i < leaf->entries.size(); ++i) {
				const KDIdx idx = leaf->entries[i];
				const Scalar value = source->kdGetValue(idx, axis);
				(KDTreeHelper::leftOf(center, value) ? newLeft : newRight)->entries.add(idx);
			}

			// the current axis does NOT provide any splitting? -> cleanup and skip this axis
			if (newLeft->entries.size() == 0 || newRight->entries.size() == 0) {
				delete newNode;
				return splitIfNeeded(leaf, nextAxis(axis), depth);
			}

			// continue with lower levels
			newNode->left =		splitIfNeeded(newLeft,  nextAxis(axis), depth + 1 );
			newNode->right =	splitIfNeeded(newRight, nextAxis(axis), depth + 1 );

//			// adjust the ownership of the lower levels
//			newNode->left->parent = newNode;
//			newNode->right->parent = newNode;

//			// reverse ownership
//			if (leaf->parent) {
//				if (leaf->parent->asNode()->left == leaf)	{((_KDTreeNode*)leaf->parent->asNode())->left = newNode;}
//				if (leaf->parent->asNode()->right == leaf)	{((_KDTreeNode*)leaf->parent->asNode())->right = newNode;}
//		}

			// delete the old leaf
			delete leaf;

			// done (replace leaf with the new node)
			return newNode;

		}

		/**
		 * check the balance for the given tree element
		 * if this is a leaf, proceed with its parent element
		 * if this is a node, check its balance
		 * if it needs a rebalance, rebuild it, otherwise proceed withi its parent element
		 */
		void checkAndBalance(_KDTreeElem* elem) {

			_KDTreeElem* needs = nullptr;

			// leaf? -> proceed with parent
			if (elem->isLeaf()) {elem = elem->parent;}

			// bubble-up from elem towards root to find the element nearest to root, that needs rebalancing
			while(elem) {
				const KDRatio ratio = getRatio(elem);
				if (ratio.getMax() > 1.25) {needs = elem; elem = elem->parent;} else {break;}
			}

			// have we found a node that needs rebalancing?
			if (needs) {rebuild((_KDTreeNode*)needs);}

		}

	private:

		/** get the next axis after the given one */
		inline int nextAxis(const int axis) const {
			return (axis + 1) % Dimensions;
		}

		/** data cleanup */
		void cleanup() {
			if (root) {delete root; root = nullptr;}
		}

		/** rebuild the given node */
		void rebuild(_KDTreeNode* node) {

			// 0) get the parent (if any)
			_KDTreeNode* parent = (_KDTreeNode*) node->parent;

			// 1) get all entries
			std::vector<KDIdx> entries;
			getElementsBelow(node, entries);

			// 2) create a new leaf
			_KDTreeLeaf* leaf = new _KDTreeLeaf(node->parent);
			for (KDIdx idx : entries) {leaf->entries.add(idx);}		// TODO faster?

			// 3) replace the old branch with the new leaf
			if (root == node) {
				root = leaf;
				root = splitIfNeeded(leaf, 0, 0);
			} else {
				if (parent->left == node) {
					parent->left = splitIfNeeded(leaf, nextAxis(parent->splitAxis), 0);		// 0??
					//parent->left->parent = parent;
				} else {
					parent->right = splitIfNeeded(leaf, nextAxis(parent->splitAxis), 0);		// 0??
					//parent->right->parent = parent;
				}
			}

		}


	};



}

#endif // K_DATA_KDTREE_KDTREE_H
