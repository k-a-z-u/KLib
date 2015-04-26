
#ifdef WITH_TESTS

#include "../../Test.h"

#include "../../../math/random/DrawWheel.h"


namespace K {

	struct MyData{
		int x;
		int y;
		MyData() : x(0), y(0) {;}
		MyData(int x, int y) : x(x), y(y) {;}
	};


	void TestDrawWheelRandom(const int numEntries) {

		DrawWheel<MyData> list;
		std::vector<double> configured;
		std::vector<double> drawn;

		const unsigned int cnt = numEntries;
		const unsigned int numDraw = cnt * 4096;
		double probSum = 0;

		list.resize(cnt);
		drawn.resize(cnt);
		configured.resize(cnt);

		// fill
		for (unsigned int i = 0; i < cnt; ++i) {
			double rnd = double(rand()) / double(RAND_MAX);
			configured[i] = rnd;
			list.set(i, MyData(i,i), rnd);
			probSum += rnd;
		}

		list.init();

		// draw
		for (unsigned int i = 0; i < numDraw; ++i) {
			MyData& d = list.draw();
			drawn[d.x]++;
		}

		// compare
		for (unsigned int i = 0; i < cnt; ++i) {
			double a = (configured[i] / probSum);
			double b = (drawn[i] / numDraw);
			ASSERT_NEAR(a, b, a*0.50);			// allow 50% difference between cfg and drawn
		}

	}

	TEST(DrawWheel, DrawTest8) {
		TestDrawWheelRandom(8);
	}

	TEST(DrawWheel, DrawTest128) {
		TestDrawWheelRandom(128);
	}

	TEST(DrawWheel, DrawTest256) {
		TestDrawWheelRandom(256);
	}

	TEST(DrawWheel, DrawTest512) {
		TestDrawWheelRandom(512);
	}





}


#endif
