#ifndef K_DATA_KDTREE_KDTREEDATA_H
#define K_DATA_KDTREE_KDTREEDATA_H

#include <vector>
#include <cstdint>
#include <cmath>
#include <algorithm>

namespace K {

	/** index-type */
	using KDIdx = uint32_t;

	template <typename Scalar> class KDTreeLeaf;
	template <typename Scalar> class KDTreeNode;

	template <typename Scalar> class KDTreeElem {

	protected:

		template <typename> friend class KDTree;

		/** is this a node or leaf? */
		const bool _isLeaf;

		/** the parent element (if any) */
		KDTreeElem<Scalar>* parent;

	public:

		/** ctor */
		KDTreeElem(const bool isLeaf, KDTreeElem* parent) : _isLeaf(isLeaf), parent(parent) {;}

		/** cast to node */
		inline const KDTreeNode<Scalar>* asNode() const {return (KDTreeNode<Scalar>*)(this);}

		/** cast to leaf */
		inline const KDTreeLeaf<Scalar>* asLeaf() const {return (KDTreeLeaf<Scalar>*)(this);}

		/** is this a node? (or a leaf) */
		inline bool isNode() const {return !_isLeaf;}

		/** is this a leaf? (or a node) */
		inline bool isLeaf() const {return _isLeaf;}

		/** get the parent-element for this element (if any) */
		inline const KDTreeElem<Scalar>* getParent() const {return parent;}

	};

	/** one node within the kd-tree */
	template <typename Scalar> struct KDTreeNode : public KDTreeElem<Scalar> {

		/** the splitting value (< = left / > = right) */
		Scalar splitValue;

		/** the splitting axis (1=x, 2=y, 3=z, ...) this splitting value belongs to */
		int splitAxis;

		/** the left subtree */
		KDTreeElem<Scalar>* left;

		/** the right subtree */
		KDTreeElem<Scalar>* right;


		/** ctor */
		KDTreeNode(const float splitValue, const int splitAxis, KDTreeElem<Scalar>* parent) :
			KDTreeElem<Scalar>(false, parent), splitValue(splitValue), splitAxis(splitAxis), left(nullptr), right(nullptr) {;}

		/** dtor */
		~KDTreeNode() {
			delete left; left = nullptr;
			delete right; right = nullptr;
		}

		/** no-copy */
		KDTreeNode(const KDTreeNode& o) = delete;

		/** no-assign */
		void operator = (const KDTreeNode& o) = delete;

		/** attach new left/right elements */
		inline void attach(KDTreeElem<Scalar>* left, KDTreeElem<Scalar>* right)  {this->left = left; this->right = right;}

		/** oldElem is a child (left/right) of this node. replace it by newElem */
		inline void switchChild(KDTreeElem<Scalar>* oldElem, KDTreeElem<Scalar>* newElem) {
			if		(left == oldElem)	{left = newElem;}
			else if (right == oldElem)	{right = newElem;}
			else						{
				throw "error!";
			}
		}

		/** get this node's parent node (if any) */
		inline KDTreeNode<Scalar>* getParent() {return (KDTreeNode<Scalar>*) KDTreeElem<Scalar>::parent;}

	};


	/** data-structure to track all entries attached to a leaf */
	class KDTreeLeafEntries {

	private:

		/** internal data structure */
		std::vector<KDIdx> indices;

	public:

		/** number of entries within this leaf */
		inline int size() const {return (int) indices.size();}

		/** get the idx-th element */
		inline KDIdx operator [] (const KDIdx idx) const {return indices[idx];}

		/** get the idx-th element */
		inline KDIdx& operator [] (const KDIdx idx) {return indices[idx];}

		/** get the data pointer */
		inline const KDIdx* data() const {return indices.data();}

		/** resize the leaf */
		inline void resize(const KDIdx size) {indices.resize(size);}

		/** resize the leaf */
		inline void reserve(const KDIdx size) {indices.reserve(size);}

		/** add the given index to the leaf */
		inline void add(const KDIdx idx) {indices.push_back(idx);}

		/** add all of the given indices to this leaf */
		inline void addAll(const std::vector<KDIdx>& vec) {indices.insert(indices.end(), vec.begin(), vec.end());}

		/** remove the given index from the leaf */
		inline void remove(const KDIdx idx) {
			indices.erase(std::remove(indices.begin(), indices.end(), idx), indices.end());
		}

	};

	/** one leaf within the kd-tree */
	template <typename Scalar> struct KDTreeLeaf : public KDTreeElem<Scalar> {

		/** indices of all elements belonging to this leaf */
		KDTreeLeafEntries entries;

		/** ctor */
		KDTreeLeaf(KDTreeElem<Scalar>* parent) : KDTreeElem<Scalar>(true, parent) {;}

		/** get this leaf's parent node (if any) */
		inline KDTreeNode<Scalar>* getParent() {return (KDTreeNode<Scalar>*) KDTreeElem<Scalar>::parent;}

	};



	/** describes one neighbor within the tree */
	template <typename Scalar> struct KDTreeNeighbor {

		/** neighbors index */
		KDIdx idx;

		/** distance from the starting element */
		Scalar distance;

		/** empty ctor */
		KDTreeNeighbor() : idx(-1), distance(-1) {;}

		/** ctor */
		KDTreeNeighbor(const KDIdx idx, const Scalar distance) : idx(idx), distance(distance) {;}

		/** for sorting */
		inline bool operator < (const KDTreeNeighbor& n) const {return distance < n.distance;}

	};

	/** ratio (left/right balance) */
	struct KDRatio {

		/** number of elements on the leaf branch */
		KDIdx numLeft;

		/** number of elements on the right branc */
		KDIdx numRight;

		/** ctor */
		KDRatio(const KDIdx numLeft, const KDIdx numRight) : numLeft(numLeft), numRight(numRight) {;}

		/** get the ratio
		 * > 1: more on the right side
		 * < 1: more on the left side
		 */
		float get() const {
		//	if (numLeft == 0 || numRight == 0) {return NAN;}
			return (float) numRight / (float) numLeft;
		}

		/** ratio numLeft/numRight for numLeft > numRight */
		float getMax() const {
		//	if (numLeft == 0 || numRight == 0) {return NAN;}
			return (float) std::max(numLeft, numRight) / (float) std::min(numLeft, numRight);
		}

		/** get the difference between numLeft and numRight as absolute value */
		KDIdx getAbsDiff() const {return static_cast<KDIdx>(std::abs( static_cast<int32_t>(numLeft) - static_cast<int32_t>(numRight)));}

		/** get the number of elements (left + right) */
		KDIdx numElements() const {return numLeft + numRight;}

	};

}

#endif // K_DATA_KDTREE_KDTREEDATA_H
