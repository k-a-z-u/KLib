#ifndef K_CL_DATA_H
#define K_CL_DATA_H

#include <cstdint>
#include <iostream>
#include <iomanip>

namespace K {
	namespace CL {


		class Data {

			CLASS_NAME("Data");

		private:

			/** data pointer */
			void* data;

			/** size of data */
			size_t size;

			/** whether to free data on dtor */
			bool freeOnDestruct;

		public:

			/** empty ctor */
			Data() : data(nullptr), size(0), freeOnDestruct(false) {
				verboseMe("empty ctor");
			}

			/** ctor with malloc request */
			Data(const size_t size) : data(nullptr), size(size), freeOnDestruct(true) {
				verboseMe("malloc ctor");
				data = malloc(size);
			}

			/** ctor from external data */
			Data(const void* data, const size_t size) : data((void*) data), size(size), freeOnDestruct(false) {
				verboseMe("ext ctor");
			}

			/** ctor from external data with destruct request */
			Data(const void* data, const size_t size, const bool freeOnDestruct) : data((void*) data), size(size), freeOnDestruct(freeOnDestruct) {
				verboseMe("ext ctor");
			}


			/** dtor */
			~Data() {
				if (freeOnDestruct) {
					verboseMe("dtor [free]");
					free(data);
					data = nullptr;
				} else {
					verboseMe("dtor [no free]");
				}
			}


			/** no-copy */
			Data(const Data& o) = delete;

			/** no-assign */
			Data& operator = (const Data& o) = delete;

			/** move ctor */
			Data(Data&& o)  : data(o.data), size(o.size), freeOnDestruct(o.freeOnDestruct) {
				verboseMe("move ctor");
				o.data = nullptr;
				o.size = 0;
				o.freeOnDestruct = false;
			}

			/** move assign*/
			Data& operator = (Data&& o) {
				verboseMe("move assign");
				this->data = o.data; o.data = nullptr;
				this->size = o.size; o.size = 0;
				this->freeOnDestruct = o.freeOnDestruct; o.freeOnDestruct = false;
				return *this;
			}

			/** access to the underlying data */
			void* getData() {return data;}

			/** const access to the underlying data */
			const void* getData() const {return data;}

			/** size of the underlying data */
			size_t getSize() const {return size;}


			/** dump as HEX */
			void dumpHEX(const size_t perLine = 8) const {
				for (size_t i = 0; i < size; ++i) {
					std::cout << std::setw(2);
					std::cout << std::setfill('0');
					std::cout << std::hex << (int) ((uint8_t*)data)[i] << ' ';
					if (i % perLine == (perLine-1)) {std::cout << std::endl;}
				}
				std::cout << std::endl;
			}

			/** dump as INT */
			void dumpINT(const size_t perLine = 8) const {
				for (size_t i = 0; i < size; ++i) {
					std::cout << std::setw(3);
					std::cout << std::setfill('0');
					std::cout << (int) ((uint8_t*)data)[i] << ' ';
					if (i % perLine == (perLine-1)) {std::cout << std::endl;}
				}
				std::cout << std::endl;
			}

		};

	}
}

#endif // DATA_H
