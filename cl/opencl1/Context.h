#ifndef K_CL_CONTEXT_H
#define K_CL_CONTEXT_H

#include "Base.h"
#include "Device.h"
#include "Platform.h"
#include "Queue.h"

#include "BufferFactory.h"
#include "ImageFactory.h"
#include "ProgramFactory.h"

#include <vector>

namespace K {
	namespace CL {

		class ContextFactory;

		class Context {

		private:

			CLASS_NAME("Context");

			/** openCL internal handle */
			cl_context ctx;

			/** all devices attached to the context */
			std::vector<Device*> attachedDevices;

			/** one command-queue per attached device */
			std::vector<Queue*> queues;

			/** create buffers that belong to this context */
			BufferFactory bufFac;

			/** create images that belong to this context */
			ImageFactory imgFac;

			/** create programs that belong to this context */
			ProgramFactory progFac;

		private:

			friend class ContextFactory;

			/** hidden ctor. use ContextFactory */
			Context() : bufFac(this), imgFac(this), progFac(this) {
				;
			}

		public:

			/** dtor */
			~Context() {
				verboseMeID(ctx, "dtor");
				clReleaseContext(ctx); ctx = 0;
				// devices are handled by the Platform class
				for (Queue* q : queues) {delete q;}
			}

			/** no-copy */
			Context(const Context& o) = delete;

			/** no-assign */
			Context& operator = (const Context& o) = delete;


			/** get the context's internal handle */
			cl_context getHandle() {
				return ctx;
			}

			/** get the BufferFactory */
			BufferFactory& buffers() {return bufFac;}

			/** get the ImageFactory */
			ImageFactory& images() {return imgFac;}

			/** get the ProgramFactory */
			ProgramFactory& programs() {return progFac;}

			/** get a list of all devices attached to the context */
			const std::vector<Device*> getAttachedDevices() {
				return attachedDevices;
			}

			/** get all command queues (one per device) */
			const std::vector<Queue*>& getCommandQueues() const {
				return queues;
			}

			/** get the CommandQueue for the idx-th device */
			Queue* getCommandQueue(const int idx) const {
				return queues[idx];
			}


			/** add a new device to this context. BEWARE: all devices MUST belong to the same platform */
			void addDevice(Device* dev) {

				// sanity check: all devices must belong to the same platform
				if (!attachedDevices.empty() && attachedDevices.front()->getPlatform() != dev->getPlatform()) {
					throw "all devices must belong to the same platform!";
				}

				// attach
				attachedDevices.push_back(dev);

			}

			/** set-up the context */
			void build() {



				// get the platform all devices belong to
				const cl_platform_id platformID = attachedDevices[0]->getPlatform()->getHandle();

				// setup context properties
				cl_context_properties properties[3] = { CL_CONTEXT_PLATFORM, (cl_context_properties) platformID, 0 };

				// setup the device-array (all devices to be used within the context)
				cl_device_id devices[100];
				for (size_t i = 0; i < attachedDevices.size(); ++i) {
					devices[i] = attachedDevices[i]->getID();
				}

				// finally, create the context;
				cl_int status;
				ctx = clCreateContext(properties, (cl_uint)attachedDevices.size(), devices, nullptr, nullptr, &status);
				check(status);

				// and: create on command-queue per device
				for (Device* dev : attachedDevices) {

					// command-queue properties
					cl_command_queue_properties qprops = 0;
					//qprops |= CL_QUEUE_PROFILING_ENABLE;		// profiling?

					// create command-queue
					cl_command_queue queue = clCreateCommandQueue(ctx, dev->getID(), qprops, &status);
					check(status);

					// store
					queues.push_back(new Queue(queue));

				}

			}

		};

	}
}

#endif // K_CL_CONTEXT_H
