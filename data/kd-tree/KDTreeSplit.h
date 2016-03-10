#ifndef K_DATA_KDTREESPLIT_H
#define K_DATA_KDTREESPLIT_H

#include <vector>
#include <set>

#include "KDTreeData.h"

namespace K {

	/** splitting methods */
	struct KDTreeSplit {

		/** use a dimension's average value for splitting */
		struct AVG {

			/** center is the average value for the given dimension */
			template <typename CFG> static inline typename CFG::Scalar getCenter(const typename CFG::DataSource* ds, const KDTreeLeafEntries& indices, const int dim, CFG c) {

				using Scalar = typename CFG::Scalar;

				Scalar sum = 0;
				for (int i = 0; i < indices.size(); ++i) {sum += ds->kdGetValue(indices[i], dim);}
				return sum / (Scalar)indices.size();

			}

		};

		/** use a dimension's approximate average value for splitting */
		struct APXAVG {

			/** center ist the approximate average for the given dimension */
			template <typename CFG> static inline typename CFG::Scalar getCenter(const typename CFG::DataSource* ds, const KDTreeLeafEntries& indices, const int dim, CFG c) {

				using Scalar = typename CFG::Scalar;

				Scalar sum = 0;
				int cnt = 0;
				const int stepSize = (indices.size() < 64) ? 1 : 4;
				for (int i = 0; i < (int)indices.size(); i+=stepSize) {sum += ds->kdGetValue(indices[i], dim); ++cnt;}
				return sum / (Scalar) cnt;

			}

		};

		/** use a dimension's median value for splitting */
		struct Median {

			/** center ist the approximate average for the given dimension */
			template <typename CFG> static inline typename CFG::Scalar getCenter(const typename CFG::DataSource* ds, const KDTreeLeafEntries& indices, const int dim) {

				using Scalar = typename CFG::Scalar;

				std::multiset<Scalar> set;
				for (int i = 0; i < indices.size(); ++i) {
					const auto val = ds->kdGetValue(indices[i], dim);
					set.insert(val);
				}

			}

		};

	};
}

#endif // K_DATA_KDTREESPLIT_H
