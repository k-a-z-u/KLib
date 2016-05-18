#ifndef K_CL_EVENT_H
#define K_CL_EVENT_H

#include "Base.h"

namespace K {
	namespace CL {

		class Event {

		private:

			cl_event event;

		public:

			/** ctor from an existing event handle */
			Event(const cl_event event) : event(event) {
				;
			}

			/** dtor */
			~Event() {
				clReleaseEvent(event);
			}

			/** no-copy */
			Event(const Event& o) = delete;

			/** move ctor */
			Event(Event&& o)  : event(o.event) {
				o.event = 0;
			}

			/** no-assign */
			Event& operator = (const Event& o) = delete;



		public:

			/** wait for this event to finish */
			void waitForCompletion() {
				check(clWaitForEvents(1, &event));
			}

		};

	}
}

#endif // K_CL_EVENT_H
