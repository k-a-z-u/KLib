#ifndef K_STREAMS_INPUTSTREAMPEEK_H
#define K_STREAMS_INPUTSTREAMPEEK_H

#include "InputStream.h"

namespace K {

	/**
	 * interface for InputStream that additionally support
	 * a peek() method to look into the next byte without
	 * consuming it.
	 */
	class InputStreamPeek : public InputStream {

	public:

		/**
		 * peek into the next available byte
		 * returns ERR_FAILED on EOF
		 * return ERR_TRY_AGAIN if nothing was available for reading
		 */
		virtual int peek() = 0;

	};

}

#endif // K_STREAMS_INPUTSTREAMPEEK_H
