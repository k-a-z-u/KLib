#ifndef TESTKDTREE_CPP
#define TESTKDTREE_CPP

#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../data/kd-tree/KDTree.h"
#include "../../../data/kd-tree/KDTreeKNN.h"
#include "../../../os/Time.h"

#include "../../../misc/gnuplot/Gnuplot.h"
#include "../../../misc/gnuplot/GnuplotPlot.h"
#include "../../../misc/gnuplot/GnuplotPlotElementPoints.h"

#include "../../../misc/GraphViz/GraphViz.h"

using namespace K;

struct KDPoint3 {

	float x;
	float y;
	float z;

	KDPoint3(const float x, const float y, const float z) : x(x), y(y), z(z) {;}

};

struct KDPointCloud : std::vector<KDPoint3> {


	/** get the distance between the given elements */
	inline float kdGetDistance(const int idxP1, const float* p2) const {
		const float dx = (*this)[idxP1].x - p2[0];
		const float dy = (*this)[idxP1].y - p2[1];
		const float dz = (*this)[idxP1].z - p2[2];
		//return std::sqrt(dx*dx + dy*dy + dz*dz);
		return std::sqrt(dx*dx + dy*dy + dz*dz);
	}

	inline float kdGetValue(const KDIdx idx, const int dim) const {
		switch(dim) {
		case 0: return (*this)[idx].x;
		case 1: return (*this)[idx].y;
		default: return (*this)[idx].z;
		}
	}

};



//TEST(KDTree, build) {

//	KDTree<float, KDPointCloud, 3> tree;

//	KDPointCloud vals;
//	vals.push_back(KDPoint3(0,0,0));
//	vals.push_back(KDPoint3(1,0,0));

//	ASSERT_EQ(tree.getRoot(), nullptr);

//	tree.build(&vals, KDTreeSplitAVG());

//	ASSERT_TRUE(tree.getRoot()->left != nullptr);
//	ASSERT_TRUE(tree.getRoot()->right != nullptr);

//	ASSERT_EQ(1, tree.getRoot()->left->asLeaf()->indices.size());
//	ASSERT_EQ(1, tree.getRoot()->right->asLeaf()->indices.size());

//	vals.push_back(KDPoint3(0,1,0));
//	vals.push_back(KDPoint3(1,1,0));

//	tree.build(&vals, KDTreeSplitAVG());

//	ASSERT_EQ(1, tree.getRoot()->left->asNode()->left->asLeaf()->indices.size());
//	ASSERT_EQ(1, tree.getRoot()->left->asNode()->right->asLeaf()->indices.size());
//	ASSERT_EQ(1, tree.getRoot()->right->asNode()->left->asLeaf()->indices.size());
//	ASSERT_EQ(1, tree.getRoot()->right->asNode()->right->asLeaf()->indices.size());

//	ASSERT_EQ(0, tree.getLeafFor({0,0,0})->indices[0]);
//	ASSERT_EQ(1, tree.getLeafFor({1,0,0})->indices[0]);
//	ASSERT_EQ(2, tree.getLeafFor({0,1,0})->indices[0]);
//	ASSERT_EQ(3, tree.getLeafFor({1,1,0})->indices[0]);

//}

//TEST(KDTree, nodes) {

//	KDTree<float, KDPointCloud, 3> tree;

//	KDPointCloud vals;
//	vals.push_back(KDPoint3(0,0,0));
//	vals.push_back(KDPoint3(1,0,0));
//	vals.push_back(KDPoint3(0,1,0));
//	vals.push_back(KDPoint3(1,1,0));

//	tree.build(&vals, KDTreeSplitAVG());

//	const KDTreeLeaf<float>* leaf = tree.getLeafFor({1,1,0});
//	ASSERT_EQ(1, tree.getNumElementsBelow(leaf));
//	ASSERT_NE(nullptr, leaf->getParent());
//	ASSERT_EQ(2, tree.getNumElementsBelow(leaf->getParent()));
//	ASSERT_NE(nullptr, leaf->getParent()->getParent());
//	ASSERT_EQ(4, tree.getNumElementsBelow(leaf->getParent()->getParent()));
//	ASSERT_EQ(nullptr, leaf->getParent()->getParent()->getParent());

//	//std::vector<int> vec;
//	//vec.clear(); tree.getElementsBelow(leaf, vec);								ASSERT_EQ(3, vec[0]);
//	//vec.clear(); tree.getElementsBelow(leaf->getParent()->getParent(), vec);	ASSERT_EQ(0, vec[0]); ASSERT_EQ(2, vec[1]); ASSERT_EQ(1, vec[2]); ASSERT_EQ(3, vec[3]);

//}

//TEST(KDTree, kNN) {

//	KDTree<float, KDPointCloud, 3> tree;

//	KDPointCloud vals;
//	vals.push_back(KDPoint3(1,0,0));
//	vals.push_back(KDPoint3(0,1,0));
//	vals.push_back(KDPoint3(0,0,1));

//	vals.push_back(KDPoint3(2,0,0));
//	vals.push_back(KDPoint3(0,2,0));
//	vals.push_back(KDPoint3(0,0,2));

//	vals.push_back(KDPoint3(1,1,0));
//	vals.push_back(KDPoint3(0,1,1));
//	vals.push_back(KDPoint3(1,0,1));

//	tree.build(&vals, KDTreeSplitAVG());

//	std::vector<KDTreeNeighbor<float>> n1 = KDTreeKNN::getNeighborsApx(tree, {0,0,0}, 3);
//	std::vector<KDTreeNeighbor<float>> n2 = KDTreeKNN::getNeighborsApx(tree, {0,0,0}, 6);
//	std::vector<KDTreeNeighbor<float>> n3 = KDTreeKNN::getNeighborsApx(tree, {0,0,0}, 9);
//	std::vector<KDTreeNeighbor<float>> n4 = KDTreeKNN::getNeighborsApx(tree, {0,0,0}, 12);

//	ASSERT_EQ(3, n1.size());
//	ASSERT_EQ(6, n2.size());
//	ASSERT_EQ(9, n3.size());
//	ASSERT_EQ(9, n4.size());

//}

//TEST(KDTree, split) {

//	KDPointCloud vals;
//	vals.push_back(KDPoint3(0,0,0));
//	vals.push_back(KDPoint3(1,0,0));
//	vals.push_back(KDPoint3(2,0,0));
//	vals.push_back(KDPoint3(3,0,0));
//	vals.push_back(KDPoint3(4,0,0));
//	vals.push_back(KDPoint3(5,0,0));
//	vals.push_back(KDPoint3(6,0,0));
//	vals.push_back(KDPoint3(7,0,0));
//	vals.push_back(KDPoint3(8,0,0));
//	vals.push_back(KDPoint3(9,0,0));

//	{
//		std::vector<KDIdx> idx = {1,5,2,9,3,6,8};
//		for (int i : idx) {std::cout << i << " ";} std::cout << std::endl;
//		KDTreeSplit::split((const KDPointCloud*) &vals, idx.data(), (KDIdx)idx.size(), 0, 4.0f);
//		for (int i : idx) {std::cout << i << " ";} std::cout << std::endl;
//	}

//	{
//		std::vector<KDIdx> idx = {8,7,6,5,  4,3,1,2};
//		for (int i : idx) {std::cout << i << " ";} std::cout << std::endl;
//		KDTreeSplit::split((const KDPointCloud*) &vals, idx.data(), (KDIdx)idx.size(), 0, 4.0f);
//		for (int i : idx) {std::cout << i << " ";} std::cout << std::endl;
//	}

//	//sleep(1000);

//}

using CFG = KDTreeConfig<float, KDPointCloud, 3, KDTreeSplit::APXAVG>;

void traverse(const KDTree<CFG>& tree, const KDTreeElem<float>* elem, std::ostream& ss) {

	if (elem->isNode()) {
		const KDTreeNode<float>* node = elem->asNode();
		ss << (size_t)node << " [label=\"" << node->splitAxis << ":" << node->splitValue << "\"]\n";
		traverse(tree, node->left, ss);
		traverse(tree, node->right, ss);
		ss << (size_t)node << " -> " << (size_t)node->left << "\n";
		ss << (size_t)node << " -> " << (size_t)node->right << "\n";
	} else {
		const KDTreeLeaf<float>* leaf = elem->asLeaf();
		ss << (size_t)leaf << " [shape=record, label=<";
		for (int i = 0; i < leaf->entries.size(); ++i) {
			if (i > 0) {ss << "|";}
//			ss << "{id:" << leaf->entries[i] << "|{";
//				for (int ax = 0; ax < 3; ++ax) {
//					if (ax > 0) {ss << "|";}
//					ss << tree.getValue(leaf->entries[i], ax);
//				}
//			ss << "}}";
			ss << "{";
				ss << "<b>" << leaf->entries[i] << "</b>";
				for (int ax = 0; ax < 3; ++ax) {
					ss << "|" << tree.getValue(leaf->entries[i], ax);
				}
			ss << "}";
		}
		ss << ">]\n";

	}


}

#include <fstream>

void toDot(const KDTree<CFG>& tree, const std::string file = "/tmp/1.png") {
	//std::ofstream ss("/tmp/1.dot");
	std::stringstream ss;
	ss << "digraph G {\n";
	traverse(tree, tree.getRoot(), ss);
	ss << "}\n";
	GraphViz gv(file); gv << ss.str();
	gv.flush();
	gv.close();
	//ss.close();
}




TEST(KDTree, add) {

	KDTree<CFG> tree(10, 4);

	std::minstd_rand gen(1234);
	std::uniform_real_distribution<float> dist(-1, +1);

	KDPointCloud vals;
	for (int i = 0; i < 1024; ++i) {
		KDPoint3 v (dist(gen), dist(gen), 0);
		vals.push_back(v);
	}

	tree.setDataSource(&vals);
	for (int i = 0; i < 1024; ++i) {
		tree.addByID(i);
		//toDot(tree);
	}



	KDTreeNode<float>* n = (KDTreeNode<float>*) tree.getRoot();

	int i = 0;

}


TEST(KDTree, balanceRemove) {

	KDTree<CFG> tree(10, 4);

	KDPointCloud vals;
	tree.setDataSource(&vals);

	for (int i = 1; i <= 32; ++i) {
		vals.push_back(KDPoint3(-i, 0, 0));
		vals.push_back(KDPoint3(+i, 0, 0));
	}

	tree.addAll(vals.size());
	toDot(tree);

	for (int i = 0; i < vals.size(); ++i) {
		tree.deleteByID(i);
		toDot(tree, "/tmp/" + std::to_string(i) + ".png");
	}

//	tree.deleteByID(7, splt);

//	toDot(tree);
//	tree.deleteByID(6, splt);
//	toDot(tree);
//	tree.deleteByID(5, splt);
//	toDot(tree);
//	tree.deleteByID(4, splt);
//	//toDot(tree);
//	//tree.deleteByID(4, splt);

}

TEST(KDTree, remove) {

	KDTree<CFG> tree(10, 4);

	KDPointCloud vals;
	vals.push_back(KDPoint3( 0, 0, 0));
	vals.push_back(KDPoint3(-1, 0, 0));
	vals.push_back(KDPoint3(+1, 0, 0));
	vals.push_back(KDPoint3( 0,-1, 0));
	vals.push_back(KDPoint3( 0,+1, 0));

	tree.setDataSource(&vals);

	tree.addByID(0);
	tree.addByID(1);
	tree.addByID(2);
	tree.addByID(3);
	tree.addByID(4);

	ASSERT_EQ(5, tree.getNumElementsBelow(tree.getRoot()));
	std::vector<KDIdx> elems;
	tree.getElementsBelow(tree.getRoot(), elems);
	ASSERT_TRUE(std::find(elems.begin(), elems.end(), 0) != elems.end());
	ASSERT_TRUE(std::find(elems.begin(), elems.end(), 1) != elems.end());
	ASSERT_TRUE(std::find(elems.begin(), elems.end(), 2) != elems.end());
	ASSERT_TRUE(std::find(elems.begin(), elems.end(), 3) != elems.end());
	ASSERT_TRUE(std::find(elems.begin(), elems.end(), 4) != elems.end());
	ASSERT_FALSE(std::find(elems.begin(), elems.end(), 5) != elems.end());

	tree.deleteByID(4);
	ASSERT_EQ(4, tree.getNumElementsBelow(tree.getRoot()));

	tree.deleteByID(4);
	ASSERT_EQ(4, tree.getNumElementsBelow(tree.getRoot()));
	elems.clear(); tree.getElementsBelow(tree.getRoot(), elems);
	ASSERT_TRUE(std::find(elems.begin(), elems.end(), 3) != elems.end());
	ASSERT_FALSE(std::find(elems.begin(), elems.end(), 4) != elems.end());

	tree.deleteByID(3);
	ASSERT_EQ(3, tree.getNumElementsBelow(tree.getRoot()));
	elems.clear(); tree.getElementsBelow(tree.getRoot(), elems);
	ASSERT_TRUE(std::find(elems.begin(), elems.end(), 2) != elems.end());
	ASSERT_FALSE(std::find(elems.begin(), elems.end(), 3) != elems.end());

	tree.deleteByID(0);
	ASSERT_EQ(2, tree.getNumElementsBelow(tree.getRoot()));
	elems.clear(); tree.getElementsBelow(tree.getRoot(), elems);
	ASSERT_TRUE(std::find(elems.begin(), elems.end(), 1) != elems.end());
	ASSERT_FALSE(std::find(elems.begin(), elems.end(), 0) != elems.end());

	tree.deleteByID(1);
	ASSERT_EQ(1, tree.getNumElementsBelow(tree.getRoot()));
	elems.clear(); tree.getElementsBelow(tree.getRoot(), elems);
	ASSERT_TRUE(std::find(elems.begin(), elems.end(), 2) != elems.end());
	ASSERT_FALSE(std::find(elems.begin(), elems.end(), 1) != elems.end());

	tree.deleteByID(2);
	ASSERT_EQ(0, tree.getNumElementsBelow(tree.getRoot()));

}



TEST(KDTree, kNNhuge) {

	KDTree<CFG> tree(10);

	std::minstd_rand gen(1234);
	std::uniform_real_distribution<float> dist(-1, +1);



	KDPointCloud vals;
	for (int i = 0; i < 1024000; ++i) {
		KDPoint3 v (dist(gen), dist(gen), 0);
		vals.push_back(v);
	}

	tree.setDataSource(&vals);
	uint64_t s1 = Time::getTimeMS();
//	tree.build(&vals, KDTreeSplitAPXAVG());
	for (int i = 0; i < 1024000; ++i) { tree.addByID(i); }
	uint64_t s2 = Time::getTimeMS();

	//std::vector<KDTreeNeighbor> n1 = KDTreeKNN::getNeighborsApx(tree, KDTreeV(0.05,0.08,0), 1000);
	for (int i = 0; i < 128; ++i) {
		KDTreeKNN::getNeighborsWithinRadius(tree, {0.05,0.08,0}, 0.15f);
	}

	uint64_t s3 = Time::getTimeMS();

	std::cout << "build: " << (s2-s1) << std::endl;
	std::cout << "search: " << (s3-s2) << std::endl;



	std::vector<KDTreeNeighbor<float>> n1 = KDTreeKNN::getNeighborsWithinRadius(tree, {0.05,0.08,0}, 0.15f);

	K::Gnuplot gp;
	K::GnuplotPlot p1;
	K::GnuplotPlotElementPoints ps1;	p1.add(&ps1);	ps1.setColorHex("#999999");
	K::GnuplotPlotElementPoints ps2;	p1.add(&ps2);	ps2.setColorHex("#000000");

	for (size_t i = 0; i < vals.size(); i += 32) {
		const KDPoint3& v = vals[i];
		ps1.add( K::GnuplotPoint2(v.x, v.y) );
	}

	for (size_t i = 0; i < n1.size(); i += 16) {
		KDTreeNeighbor<float> n = n1[i];
		ps2.add( K::GnuplotPoint2(vals[n.idx].x, vals[n.idx].y) );
	}

	gp.draw(p1);
	gp.flush();;
	sleep(100);

}

#endif // WITH_TESTS

#endif // TESTKDTREE_CPP
