#ifndef K_CL_EVENT_H
#define K_CL_EVENT_H

#include "Base.h"

namespace K {
	namespace CL {

		class Event {

			CLASS_NAME("Event");

		private:

			cl_event event;

		public:

			/** ctor from an existing event handle */
			Event(const cl_event event) : event(event) {
				verboseMeID(event, "create");
			}

			/** dtor */
			~Event() {
				verboseMeID(event, "release");
				clReleaseEvent(event);
			}

			/** no-copy */
			Event(const Event& o) = delete;

			/** no-assign */
			Event& operator = (const Event& o) = delete;

			/** move ctor */
			Event(Event&& o)  : event(o.event) {
				verboseMe("move ctor");
				o.event = 0;
			}

			/** move assign */
			Event& operator = (Event&& o) {
				verboseMe("move assign");
				this->event = o.event; o.event = nullptr;
				return *this;
			}





		public:

			/** wait for this event to finish */
			void waitForCompletion() {
				check(clWaitForEvents(1, &event));
			}

		};

	}
}

#endif // K_CL_EVENT_H
