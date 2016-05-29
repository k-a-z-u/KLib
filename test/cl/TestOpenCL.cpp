#ifdef WITH_TESTS

#include "../Test.h"
#include "../../cl/opencl1/CL.h"
#include "../../fs/File.h"

using namespace K::CL;

TEST(OpenCL, listDevices) {

	System sys;

	for (Platform* p : sys.getPlatforms()) {
		std::cout << (std::string) *p << std::endl;
		for (Device* dev : p->getDevices()) {
			std::cout << "\t" << (std::string) *dev << std::endl;
			dev->dumpAttributes();
		}
	}

}

TEST(OpenCL, kernelInclude1) {

	System sys;

	Context* ctx = sys.contexts().create();
	ctx->addDevice(sys.getPlatform(0)->getDevice(0));
	ctx->build();


	const std::string code = "\
			#include \"Inc.h\"\
			__kernel void run() {\
				int x = get();\
			}";


	K::File file(__FILE__);
	K::File folder = file.getParent();

	Program* prog = ctx->programs().create();

	// set the include-path to see "Inc.h"
	prog->addIncludePath(folder.getAbsolutePath());
	prog->setSource(code);

}

TEST(OpenCL, kernelInclude2) {

	System sys;

	Context* ctx = sys.contexts().create();
	ctx->addDevice(sys.getPlatform(0)->getDevice(0));
	ctx->build();

	// get source-file
	K::File file(__FILE__);
	K::File folder = file.getParent();
	K::File src(folder, "Test.cl.h");

	Program* prog = ctx->programs().create();
	prog->addDefinition("DX", "1");
	prog->addDefinition("DY", "2");
	prog->setSourceFromFile(src.getAbsolutePath());

	//exit(0);

}

TEST(OpenCL, image) {

	System sys;

	Context* ctx = sys.contexts().create();
	ctx->addDevice(sys.getPlatform(0)->getDevice(0));
	ctx->build();

	// create a kernel
	const std::string code = "\
			__kernel void run(__read_only image2d_t img1, __read_only image2d_t img2, __write_only image2d_t img3) {\
				int2 pos = (int2)(get_global_id(0), get_global_id(1));\
				float4 v1 = read_imagef(img1, pos);\
				float4 v2 = read_imagef(img2, pos);\
				float4 v3 = v1+v2;\
				write_imagef(img3, pos, v3);\
			}";

	Program* prog = ctx->programs().createFromSource(code);
	Kernel* kernel = prog->kernels().create("run");

	uint8_t d1[] =	{4, 4, 4, 4,
					  3, 3, 3, 3,
					  2, 2, 2, 2,
					  1, 1, 1, 1};

	uint8_t d2[] =	{1, 2, 3, 4,
					  4, 3, 2, 1,
					  5, 6, 7, 8,
					  8, 7, 6, 5};

	uint8_t d3[16];

	Image* img1 = ctx->images().createReadOnly(ImageDesc(4,4), ImageFormat::GRAY_UINT8, d1, 16);
	Image* img2 = ctx->images().createReadOnly(ImageDesc(4,4), ImageFormat::GRAY_UINT8, d2, 16);
	Image* img3 = ctx->images().createWriteOnly(ImageDesc(4,4), ImageFormat::GRAY_UINT8, d3, 16);

	// bind
	kernel->setArg(0, img1);
	kernel->setArg(1, img2);
	kernel->setArg(2, img3);

	// upload
	img1->upload(ctx->getCommandQueue(0));
	img2->upload(ctx->getCommandQueue(0));

	// run
	kernel->run(ctx->getCommandQueue(0), 2, Range(8,4), Range(2,2));

	// download
	Event evtFetch = img3->download(ctx->getCommandQueue(0));
	evtFetch.waitForCompletion();

	// test the result
	img3->getData().dumpINT(4);
	uint8_t cmp[] = {5,6,7,8, 7,6,5,4, 7,8,9,10, 9,8,7,6};
	ASSERT_ARRAY_EQ(cmp, (uint8_t*)img3->getData().getData(), 16);

}



TEST(OpenCL, buffer) {

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

	Context* ctx = sys.contexts().create();
	ctx->addDevice(sys.getPlatform(0)->getDevice(0));
	ctx->build();

	Program* prog = ctx->programs().createFromSource(code);
	Kernel* kernel = prog->kernels().create("callMe");

	uint8_t _img1[8*4];
	uint8_t _img2[8*4];

	Buffer* img1 = ctx->buffers().createReadOnly(_img1, 32);
	Buffer* img2 = ctx->buffers().createReadOnly(_img2, 32);
	Buffer* out = ctx->buffers().createWriteOnly(32);

	kernel->setArg(0, img1);
	kernel->setArg(1, img2);
	kernel->setArg(2, out);

	Event evt = kernel->run(ctx->getCommandQueue(0), 2, Range(8,4), Range(2,2));
	evt.waitForCompletion();
	out->getData().dumpINT();

	Event evtFetch = out->download(ctx->getCommandQueue(0));
	evtFetch.waitForCompletion();

	out->getData().dumpINT();

	int i = 0; (void) i;

}

#endif
