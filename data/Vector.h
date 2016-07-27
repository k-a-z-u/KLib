#ifndef VECTOR_H
#define VECTOR_H

#include <vector>

namespace K {

	/** helper methods for std::vector */
	class Vector {

	public:

		/** build the average of all elements within the given vector */
		template <typename Element> static Element average(const std::vector<Element>& vec) {
			Element sum;
			for (const Element& e : vec) {sum += e;}
			return sum / vec.size();
		}

	};

}

#endif // VECTOR_H
