#ifndef K_DATA_KDTREE_KDTREEKNN_H
#define K_DATA_KDTREE_KDTREEKNN_H

#include <vector>
#include <set>

#include "KDTree.h"
#include "KDTreeHelper.h"

namespace K {

	/**
	 * k-nearest-neighbor search within KD-Trees
	 */
	class KDTreeKNN {

	public:

		/** get approximate nearest neighbors for the given coordinates */
		template <typename CFG>
		static std::vector<KDTreeNeighbor<typename CFG::Scalar>> getNeighborsApx(const KDTree<CFG>& tree, const std::initializer_list<typename CFG::Scalar> values, const KDIdx num) {
			return getNeighborsApx(tree, values.begin(), num);
		}

		/** get approximate nearest neighbors for the given coordinates */
		template <typename CFG>
		static std::vector<KDTreeNeighbor<typename CFG::Scalar>> getNeighborsApx(const KDTree<CFG>& tree, const typename CFG::Scalar search[CFG::Dimensions], const KDIdx num) {

			using Scalar = typename CFG::Scalar;

			// neighbors sorted by distance
			std::set<KDTreeNeighbor<Scalar>> neighbors;

			// get the leaf-node "elem" would belong to
			const KDTreeLeaf<Scalar>* leaf = tree.getLeafFor(search);

			// add all elements
			for (int i = 0; i < leaf->indices.size(); ++i) {
				const KDIdx idx = leaf->indices[i];
				KDTreeNeighbor<Scalar> knn(idx, tree.getDistance(idx, search));
				neighbors.insert(knn);
			}

			// limit
			std::vector<KDTreeNeighbor<Scalar>> out;
			for (auto it : neighbors) {
				out.push_back(it);
				if ((KDIdx) out.size() >= num)		{break;}
				if (out.size() >= neighbors.size())	{break;}
			}

			// done
			return out;

		}

		/** fetch all neighbors near elem within the given radius */
		template <typename CFG>
		static std::vector<KDTreeNeighbor<typename CFG::Scalar>> getNeighborsWithinRadius(const KDTree<CFG>& tree, const std::initializer_list<typename CFG::Scalar> search, const typename CFG::Scalar radius) {
			return getNeighborsWithinRadius(tree, search.begin(), radius);
		}

		/** fetch all neighbors near elem within the given radius */
		template <typename CFG>
		static std::vector<KDTreeNeighbor<typename CFG::Scalar>> getNeighborsWithinRadius(const KDTree<CFG>& tree, const typename CFG::Scalar search[CFG::Dimensions], const typename CFG::Scalar radius) {

			using Scalar = typename CFG::Scalar;

			// will traverse the tree to find the leaf the search-node belongs to.
			// thereby also looks at all opposite halfs, IF they are within the radius
			// and there is a chance for them to contain additional neighbors

			// start
			const KDTreeElem<Scalar>* root = tree.getRoot();

			// output
			std::vector<KDTreeNeighbor<Scalar>> nn;

			// run
			visit(tree, root, search, radius, nn);

			// done
			return nn;

		}

	private:

		/** helper method for k-NN above */
		template <typename CFG>
		static inline void visit(const KDTree<CFG>& tree, const KDTreeElem<typename CFG::Scalar>* elem, const typename CFG::Scalar search[CFG::Dimensions], const typename CFG::Scalar radius, std::vector<KDTreeNeighbor<typename CFG::Scalar>>& nn) {

			using Scalar = typename CFG::Scalar;

			if (elem->isNode()) {

				// check whether to process the left or the right branch of this node
				const KDTreeNode<Scalar>* node = elem->asNode();
				const Scalar dist = std::abs(search[node->splitAxis] - node->splitValue);

				// left or right half?
				if (KDTreeHelper::leftOf(node, search)) {
					visit(tree, node->left, search, radius, nn);						// definitely visit the left half!
					if (dist < radius) {visit(tree, node->right, search, radius, nn);}	// also visit the right half as its within the radius
				} else {
					visit(tree, node->right, search, radius, nn);						// definitely visit the right half!
					if (dist < radius) {visit(tree, node->left, search, radius, nn);}	// also visit the left half as its within the radius
				}

			} else {

				// add all leaf-elements within the search-radius
				const KDTreeLeaf<Scalar>* leaf = elem->asLeaf();
				//for (const KDIdx idx : leaf->indices) {
				for (KDIdx i = 0; i < leaf->entries.size(); ++i) {
					const KDIdx idx = leaf->entries[i];
					const Scalar dist = tree.getDistance(idx, search);
					if (dist <= radius) {nn.push_back(KDTreeNeighbor<Scalar>(idx,dist));}
				}

			}

		}

	};

}

#endif // K_DATA_KDTREE_KDTREEKNN_H
