/*
 * Buffer.h
 *
 *  Created on: Dec 18, 2013
 *      Author: kazu
 */

#ifndef BUFFER_H_
#define BUFFER_H_



namespace K {

	/**
	 * this class represents a buffer to store elements of a given type.
	 * the buffer uses a continuous memory region during all operations.
	 *
	 * in order to speed things up, free memory is only reclaimed after
	 * a certain number of free bytes is reached. otherwise we would move
	 * the memory every time we remove a byte, which is quite awful...
	 *
	 */
	template <typename T> class Buffer {

	private:

		/** the internal buffer */
		T* _buf;

		/** pointer to the first used byte (free space at the "left side" of the buffer) */
		T* firstUsed;

		/** pointer to the first free byte (at the "right side" of the buffer) */
		T* firstFree;

		/** index of the oldest available entry */
		unsigned int freeAtFront;

		/** the number of used entries */
		unsigned int usedEntries;

		/** the total number of slots within the buffer */
		unsigned int totalEntries;


		/** reclaim free space at the buffer's left side, after at least 4096 bytes are free */
		static const unsigned int FREE_LIMIT = 4096;


	public:

		/** ctor */
		Buffer() : _buf(nullptr), freeAtFront(0), usedEntries(0), totalEntries(0) {
			firstFree = _buf;
			firstUsed = _buf;
		}

		/** dtor */
		~Buffer() {
			free(_buf);
			_buf = nullptr;
			firstFree = nullptr;
			firstUsed = nullptr;
		}

		/** add the given elements */
		void add(const T* elems, unsigned int numElems) {
			ensureSpace(numElems);
			memcpy(getFirstFree(), elems, numElems * sizeof(T));
			firstFree += numElems;
			usedEntries += numElems;
		}

		/** add the given element */
		void add(T elem) {
			ensureSpace(1);
			*getFirstFree() = elem;
			++firstFree;
			++usedEntries;
		}

		/** get the first entry from the buffer and remove it */
		T get() {
			T ret = firstUsed[0];//buffer[freeAtFront];
			remove(1);
			return ret;
		}


		/**
		 * remove the given number of entries from
		 * the front of the buffer
		 */
		void remove (unsigned int numEntries) {
			if (numEntries == 0) {return;}
			if (numEntries == usedEntries) {clear(); return;}
			if (usedEntries < numEntries) {throw "out of bounds";}
			usedEntries -= numEntries;
			freeAtFront += numEntries;
			firstUsed += numEntries;
			if (freeAtFront > FREE_LIMIT) {removeFreeAtFront();}
		}

		/**
		 * manually overwrite the number of used entries e.g.
		 * when using memset to write to the buffer
		 */
		void setNumUsed(unsigned int numUsed) {
			if (numUsed > totalEntries - freeAtFront) {throw "out of bounds";}
			int32_t delta = (int32_t) numUsed - (int32_t) usedEntries;
			usedEntries += delta;
			firstFree += delta;
		}

		/**
		 * get the number of used entries.
		 * the occupied memory depends on the number of entries
		 * and the size of the buffered data-type.
		 */
		unsigned int getNumUsed() const {
			return usedEntries;
		}

		/** is the buffer currently empty? */
		bool empty() const {
			return usedEntries == 0;
		}


		/**
		 * get the number of free entries available for writing
		 */
		unsigned int getNumFree() const {
			return totalEntries - usedEntries - freeAtFront;
		}

		/** get a pointer to the first free entry */
		T* getFirstFree() const {
			return firstFree;
		}

		/**
		 * get the data as pointer.
		 * used getNumUsed() the check the number of available entries
		 */
		T* getData() const {
			return firstUsed;
		}

		/** get the memory consumption (in bytes) */
		unsigned int getMemoryConsumption() const {
			return totalEntries * (unsigned int) sizeof(T);
		}

		/** get the number of allocated entries */
		unsigned int getSize() const {
			return totalEntries;
		}


		/** resize the buffer to hold the given number of elements */
		void resize(unsigned int numElements) {

			// ensure we remove the free-space at the beginning first
			removeFreeAtFront();

			// resize
			T* newBuffer = (T*) realloc(_buf, numElements * sizeof(T));
			if (!newBuffer) {throw "out of memory";}

			// adjust all pointers
			int64_t delta = newBuffer - _buf;
			_buf += delta;
			firstFree += delta;
			firstUsed += delta;

			// done
			totalEntries = numElements;

		}

		/**
		 * ensure the buffers size is at least the given number of elements.
		 * if the buffer is already larger than the given number, nothing
		 * will happen. if the buffer is smaller, it will be resized.
		 */
		void ensureMinSize(unsigned int numElements) {
			if (totalEntries - freeAtFront >= numElements) {return;}
			resize(numElements);
		}

		/** access elements by array index */
		T& operator[] (const unsigned int index) {
			return firstUsed[index];
		}

		/**
		 * clear the buffer.
		 * this will simply reset the number of used entries.
		 * the allocated memory will not be changed at all.
		 */
		void clear() {
			usedEntries = 0;
			freeAtFront = 0;
			firstUsed = _buf;
			firstFree = _buf;
		}



	private:


		/** remove the free bytes at the left side of the buffer to reclaim the free memory */
		void removeFreeAtFront() {
			//std::cout << "removing free front space" << std::endl;
			memmove(&_buf[0], &firstUsed[0], (usedEntries) * sizeof(T));
			firstUsed -= freeAtFront;
			firstFree -= freeAtFront;
			freeAtFront = 0;
		}

		/** ensure the buffer can pack the given number of elements */
		void ensureSpace(unsigned int numElems) {

			// already enough space?
			if ((totalEntries - usedEntries - freeAtFront) > numElems) {return;}

			// try doubling the buffer's size
			unsigned int newSize = (unsigned int) totalEntries * 2;
			if (newSize - usedEntries - freeAtFront < numElems) {newSize = usedEntries + numElems + freeAtFront;}

			// resize the buffer
			resize(newSize);

		}


	};


}



//namespace K {

//template <typename T> class Buffer {

//public:

//	/** ctor */
//	Buffer() : buffer(nullptr), usedEntries(0), totalEntries(0) {
//		;
//	}

//	/** dtor */
//	~Buffer() {
//		free(buffer);
//		buffer = nullptr;
//	}

//	/** add the given elements */
//	void add(const T* elems, unsigned int numElems) {
//		ensureSpace(numElems);
//		memcpy(&buffer[usedEntries], elems, numElems * sizeof(T));
//		usedEntries += numElems;
//	}

//	/** add the given element */
//	void add(T elem) {
//		ensureSpace(1);
//		buffer[usedEntries] = elem;
//		++usedEntries;
//	}

//	/** get the first entry from the buffer and remove it */
//	T get() {
//		T ret = buffer[0];
//		remove(1);
//		return ret;
//	}


//	/**
//	 * remove the given number of entries from
//	 * the front of the buffer
//	 */
//	void remove (unsigned int numEntries) {
//		if (numEntries == 0) {return;}
//		if (numEntries == usedEntries) {clear(); return;}
//		if (usedEntries < numEntries) {throw "out of bounds";}
//		memmove(&buffer[0], &buffer[numEntries], (usedEntries - numEntries) * sizeof(T));
//		usedEntries -= numEntries;
//	}

//	/**
//	 * manually overwrite the number of used entries e.g.
//	 * when using memset to write to the buffer
//	 */
//	void setNumUsed(unsigned int numUsed) {
//		usedEntries = numUsed;
//	}

//	/**
//	 * get the number of used entries.
//	 * the occupied memory depends on the number of entries
//	 * and the size of the buffered data-type.
//	 */
//	unsigned int getNumUsed() const {
//		return usedEntries;
//	}

//	/** is the buffer currently empty? */
//	bool empty() const {
//		return usedEntries == 0;
//	}


//	/**
//	 * get the number of free entries available for writing
//	 */
//	unsigned int getNumFree() const {
//		return totalEntries - usedEntries;
//	}

//	/** get a pointer to the first free entry */
//	T* getFirstFree() const {
//		return &buffer[usedEntries];
//	}

//	/**
//	 * get the data as pointer.
//	 * used getNumUsed() the check the number of available entries
//	 */
//	T* getData() const {
//		return &buffer[0];
//	}

//	/** get the memory consumption (in bytes) */
//	unsigned int getMemoryConsumption() const {
//		return totalEntries * (unsigned int) sizeof(T);
//	}

//	/** get the number of allocated entries */
//	unsigned int getSize() const {
//		return totalEntries;
//	}


//	/** resize the buffer to hold the given number of elements */
//	void resize(unsigned int numElements) {

//		// resize
//		buffer = (T*) realloc(buffer, numElements * sizeof(T));
//		if (!buffer) {throw "out of memory";}
//		totalEntries = numElements;

//	}

//	/**
//	 * ensure the buffers size is at least the given number of elements.
//	 * if the buffer is already larger than the given number, nothing
//	 * will happen. if the buffer is smaller, it will be resized.
//	 */
//	void ensureMinSize(unsigned int numElements) {
//		if (totalEntries >= numElements) {return;}
//		resize(numElements);
//	}

//	/** access elements by array index */
//	T& operator[] (const unsigned int index) {
//		return buffer[index];
//	}

//	/**
//	 * clear the buffer.
//	 * this will simply reset the number of used entries.
//	 * the allocated memory will not be changed at all.
//	 */
//	void clear() {
//		usedEntries = 0;
//	}



//private:


//	/** ensure the buffer can pack the given number of elements */
//	void ensureSpace(unsigned int numElems) {

//		// already enough space?
//		if ((totalEntries - usedEntries) > numElems) {return;}

//		// try doubling the buffer's size
//		unsigned int newSize = (unsigned int) totalEntries * 2;
//		if (newSize - usedEntries < numElems) {newSize = usedEntries + numElems;}

//		// resize the buffer
//		resize(newSize);

//	}

//	/** the internal buffer */
//	T* buffer;

//	/** index of the first, used item. we use this to avoid many reallocs */
//	unsigned int firstUsed;

//	/** the number of used entries */
//	unsigned int usedEntries;

//	/** the total number of slots within the buffer */
//	unsigned int totalEntries;

//};


//}

#endif /* BUFFER_H_ */
