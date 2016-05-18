#ifndef CONTEXT_H
#define CONTEXT_H

#include "Base.h"
#include "Device.h"
#include "Platform.h"

#include <vector>


namespace CL {

	class Context {

	private:

		cl_context ctx;
		cl_command_queue queue;

		/** all devices attached to the context */
		std::vector<Device*> attachedDevices;

		/** one command-queue per attached device */
		std::vector<cl_command_queue> queues;

	public:

		/** ctor */
		Context() {

			create();

		}

		/** get the context's internal ID */
		cl_context getID() {
			return ctx;
		}

		/** get a list of all devices attached to the context */
		const std::vector<Device*> getAttachedDevices() {
			return attachedDevices;
		}

		/** get all command queues (one per device) */
		const std::vector<cl_command_queue>& getCommandQueues() {
			return queues;
		}

		/** add a new device to this context. BEWARE: all devices MUST belong to the same platform */
		void add(Device* dev) {

			// sanity check: all devices must belong to the same platform
			if (!attachedDevices.empty() && attachedDevices.front()->getPlatform() != dev->getPlatform()) {
				throw "all devices must belong to the same platform!";
			}

			// attach
			attachedDevices.push_back(dev);

		}

		/** set-up the context */
		void build() {

			cl_int status;

			// get the platform all devices belong to
			const cl_platform_id platformID = attachedDevices[0]->getPlatform()->getID();

			// setup context properties
			cl_context_properties properties[3] = { CL_CONTEXT_PLATFORM, (cl_context_properties) platformID, 0 };

			// setup the device-array (all devices to be used within the context)
			cl_device_id devices[100];
			for (size_t i = 0; i < attachedDevices.size(); ++i) {
				devices[i] = attachedDevices[i]->getID();
			}

			// finally, create the context;
			ctx = clCreateContext(properties, attachedDevices.size(), devices, nullptr, nullptr, &status);
			check(status);

			// and: create on command-queue per device
			for (Device* dev : attachedDevices) {

				// command-queue properties
				cl_command_queue_properties qprops = 0;
				qprops |= CL_QUEUE_PROFILING_ENABLE;		// profiling?

				// create command-queue
				queue = clCreateCommandQueue(ctx, dev->getID(), qprops, &status);
				check(status);

				// store
				queues.push_back(queue);

			}

		}

	private:

		void create() {

		}



	};

}

#endif // CONTEXT_H
