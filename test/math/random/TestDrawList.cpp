
#ifdef WITH_TESTS

#include "../../Test.h"

#include "../../../math/random/DrawList.h"


namespace K {

	struct MyData{
		int x;
		int y;
		MyData() : x(0), y(0) {;}
		MyData(int x, int y) : x(x), y(y) {;}
	};

	TEST(DrawList, draw) {

		DrawList<MyData> list;
		list.push_back(MyData(0,1), 0.1);
		list.push_back(MyData(1,2), 0.1);
		list.push_back(MyData(2,3), 0.1);
		list.push_back(MyData(3,4), 0.1);
		list.push_back(MyData(4,6), 0.2);
		list.push_back(MyData(6,10), 0.4);

		ASSERT_EQ(0, list.draw(0.00).x);
		ASSERT_EQ(0, list.draw(0.05).x);
		ASSERT_EQ(0, list.draw(0.09).x);

		ASSERT_EQ(1, list.draw(0.11).x);
		ASSERT_EQ(1, list.draw(0.15).x);
		ASSERT_EQ(1, list.draw(0.19).x);

		ASSERT_EQ(2, list.draw(0.201).x);
		ASSERT_EQ(2, list.draw(0.25).x);
		ASSERT_EQ(2, list.draw(0.299).x);

		ASSERT_EQ(3, list.draw(0.30001).x);
		ASSERT_EQ(3, list.draw(0.3333).x);
		ASSERT_EQ(3, list.draw(0.399999).x);

		ASSERT_EQ(4, list.draw(0.40001).x);
		ASSERT_EQ(4, list.draw(0.4333).x);
		ASSERT_EQ(4, list.draw(0.599999).x);

		ASSERT_EQ(6, list.draw(0.600001).x);
		ASSERT_EQ(6, list.draw(0.7).x);
		ASSERT_EQ(6, list.draw(0.999999).x);

	}



	/** assert that cumulative values are correctly calculated when drawing the first time */
	TEST(DrawList, Cumulative) {

		DrawList<MyData> list;
		list.push_back(MyData(), 0.11);
		list.push_back(MyData(), 0.22);
		list.push_back(MyData(), 0.33);
		list.push_back(MyData(), 0.44);
		list.push_back(MyData(), 0.55);
		list.push_back(MyData(), 0.66);

		for (DrawListEntry<MyData>& dle : list.entries) {
			ASSERT_EQ(0, dle.cumulativeProbability);
		}

		list.draw();

		ASSERT_FLOAT_EQ(0.11, list.entries[0].cumulativeProbability);
		ASSERT_FLOAT_EQ(0.33, list.entries[1].cumulativeProbability);
		ASSERT_FLOAT_EQ(0.66, list.entries[2].cumulativeProbability);
		ASSERT_FLOAT_EQ(1.10, list.entries[3].cumulativeProbability);
		ASSERT_FLOAT_EQ(1.65, list.entries[4].cumulativeProbability);
		ASSERT_FLOAT_EQ(2.31, list.entries[5].cumulativeProbability);

		list.set(0, MyData(), 0.22);

		ASSERT_FLOAT_EQ(0.00, list.entries[0].cumulativeProbability);
		ASSERT_FLOAT_EQ(0.33, list.entries[1].cumulativeProbability);
		ASSERT_FLOAT_EQ(0.66, list.entries[2].cumulativeProbability);
		ASSERT_FLOAT_EQ(1.10, list.entries[3].cumulativeProbability);
		ASSERT_FLOAT_EQ(1.65, list.entries[4].cumulativeProbability);
		ASSERT_FLOAT_EQ(2.31, list.entries[5].cumulativeProbability);

		list.draw();

		ASSERT_FLOAT_EQ(0.22, list.entries[0].cumulativeProbability);
		ASSERT_FLOAT_EQ(0.44, list.entries[1].cumulativeProbability);
		ASSERT_FLOAT_EQ(0.77, list.entries[2].cumulativeProbability);
		ASSERT_FLOAT_EQ(1.21, list.entries[3].cumulativeProbability);
		ASSERT_FLOAT_EQ(1.76, list.entries[4].cumulativeProbability);
		ASSERT_FLOAT_EQ(2.42, list.entries[5].cumulativeProbability);

	}


	void TestDrawListRandom(int numEntries) {

		DrawList<MyData> list;
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

	TEST(DrawList, DrawTest8) {
		TestDrawListRandom(8);
	}

	TEST(DrawList, DrawTest128) {
		TestDrawListRandom(128);
	}

	TEST(DrawList, DrawTest256) {
		TestDrawListRandom(256);
	}

	TEST(DrawList, DrawTest512) {
		TestDrawListRandom(512);
	}





}


#endif
