#ifdef WITH_TESTS

#include "../Test.h"
#include "../../cl/opencl1/System.h"
#include "../../cl/opencl1/Context.h"
#include "../../cl/opencl1/Kernel.h"
#include "../../cl/opencl1/Range.h"

using namespace K::CL;

TEST(OpenCL, list) {

	const std::string code = "\
			__kernel void callMe(__global const char* img1, __global const char* img2, __global char* res) {\
				const int w = get_global_size(0);\
				const int h = get_global_size(1);\
				const int x = get_global_id(0);\
				const int y = get_global_id(1);\
				const int idx = x+y*w;\
				res[idx] = idx;\
			}";

	System sys;
	Context ctx;

	for (Platform* p : sys.getPlatforms()) {
		std::cout << (std::string) *p << std::endl;
		for (Device* dev : p->getDevices()) {
			std::cout << "\t" << (std::string) *dev << std::endl;
			ctx.addDevice(dev);
		}
	}

	ctx.build();

	Program* prog = new Program(&ctx);
	prog->setSource(code);

	Kernel* kernel = new Kernel(prog, "callMe");

	uint8_t _img1[8*4];
	uint8_t _img2[8*4];

	Buffer* img1 = Buffer::createReadOnly(&ctx, _img1, 32);
	Buffer* img2 = Buffer::createReadOnly(&ctx, _img2, 32);
	Buffer* out = Buffer::createWriteOnly(&ctx, 32);

	kernel->setArg(0, img1);
	kernel->setArg(1, img2);
	kernel->setArg(2, out);

	Event evt = kernel->run(ctx.getCommandQueues()[0], 2, Range(8,4), Range(2,2));
	evt.waitForCompletion();
	out->dumpINT();

	Event evtFetch = out->download(ctx.getCommandQueues()[0]);
	evtFetch.waitForCompletion();
	//out->dumpINT();

	int i = 0; (void) i;

}

#endif
