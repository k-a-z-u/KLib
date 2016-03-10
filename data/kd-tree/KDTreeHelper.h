#ifndef K_DATA_KDTREE_KDTREEHELPER_H
#define K_DATA_KDTREE_KDTREEHELPER_H

#include "KDTreeData.h"

namespace K {

	/** some helper methods */
	struct KDTreeHelper {

		/** do the given scalar values belong to the left or the right branch of the given node? */
		template <typename Scalar>
		static inline bool leftOf(const KDTreeNode<Scalar>* node, const Scalar* values) {
			return values[node->splitAxis] <= node->splitValue;
		}

		/** does the given scalar value belong to the left or the right branch of the given center? */
		template <typename Scalar>
		static inline bool leftOf(const Scalar nodeValue, const Scalar userValue) {
			return userValue <= nodeValue;
		}

	};

}

#endif // K_DATA_KDTREE_KDTREEHELPER_H
