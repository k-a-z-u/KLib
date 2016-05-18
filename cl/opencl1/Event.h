#ifndef EVENT_H
#define EVENT_H

#include "Base.h"

namespace CL {

	class Event {

	private:

		cl_event event;

	public:

		/** ctor from an existing event handle */
		Event(const cl_event event) : event(event) {
			;
		}

		/** wait for this event to finish */
		void waitForCompletion() {
			check(clWaitForEvents(1, &event));
		}

	};

}

#endif // EVENT_H
