#ifndef BUFFERFACTORY_H
#define BUFFERFACTORY_H

#include <vector>
#include <cstdint>

namespace K {
	namespace CL {

		class Buffer;
		class Context;

		/**
		 * create data-buffers that belong to a context
		 */
		class BufferFactory {

		private:

			/** the context the BufferFactory belongs to */
			Context* ctx;

			/** all buffers created by the factory */
			std::vector<Buffer*> buffers;

		private:

			friend class Context;

			/** hidden ctor */
			BufferFactory(Context* ctx);

		public:

			/** dtor */
			~BufferFactory();

			/** manually delete the given buffer */
			void dispose(Buffer* buf);

			/** create a read-only buffer */
			Buffer* createReadOnly(const void* data, const size_t size);

			/** create a write-only buffer */
			Buffer* createWriteOnly(const size_t size);


		};

	}
}

#endif // BUFFERFACTORY_H
