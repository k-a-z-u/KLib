#ifdef WITH_TESTS

#include "../../Test.h"

#include "../../../math/graph/Graph.h"

namespace K {

	TEST(Graph, createDelete) {
		Graph<Vertex, Edge>* g = new Graph<Vertex, Edge>();
		g->createNewVertex();
		g->createNewEdge();
	}

	TEST(Graph, Assign) {

		Graph<Vertex, Edge>* g = new Graph<Vertex, Edge>();
		Vertex* v1 = g->createNewVertex();
		Vertex* v2 = g->createNewVertex();
		Edge* e = g->createNewEdge(v1, v2);

		// set?
		ASSERT_EQ(e->v1, v1);
		ASSERT_EQ(e->v2, v2);
		ASSERT_EQ(1, v1->edges.size());
		ASSERT_EQ(1, v2->edges.size());

		// remove 1
		e->disconnect(v1);
		ASSERT_EQ(e->v1, nullptr);
		ASSERT_EQ(e->v2, v2);
		ASSERT_EQ(0, v1->edges.size());
		ASSERT_EQ(1, v2->edges.size());

		// remove 2
		e->disconnect(v2);
		ASSERT_EQ(e->v1, nullptr);
		ASSERT_EQ(e->v2, nullptr);
		ASSERT_EQ(0, v1->edges.size());
		ASSERT_EQ(0, v2->edges.size());

		// set 2
		e->setV2(v2);
		ASSERT_EQ(e->v1, nullptr);
		ASSERT_EQ(e->v2, v2);
		ASSERT_EQ(0, v1->edges.size());
		ASSERT_EQ(1, v2->edges.size());

		// set 1
		e->setV1(v1);
		ASSERT_EQ(e->v1, v1);
		ASSERT_EQ(e->v2, v2);
		ASSERT_EQ(1, v1->edges.size());
		ASSERT_EQ(1, v2->edges.size());

		// set twice
		e->setV2(v2);
		ASSERT_EQ(e->v1, v1);
		ASSERT_EQ(e->v2, v2);
		ASSERT_EQ(1, v1->edges.size());
		ASSERT_EQ(1, v2->edges.size());

		// set twice
		e->setV1(v1);
		ASSERT_EQ(e->v1, v1);
		ASSERT_EQ(e->v2, v2);
		ASSERT_EQ(1, v1->edges.size());
		ASSERT_EQ(1, v2->edges.size());

		// disconnect
		e->disconnect();
		ASSERT_EQ(e->v1, nullptr);
		ASSERT_EQ(e->v2, nullptr);
		ASSERT_EQ(0, v1->edges.size());
		ASSERT_EQ(0, v2->edges.size());

		// reconnect the other way round
		e->setV1(v2);
		e->setV2(v1);
		ASSERT_EQ(e->v1, v2);
		ASSERT_EQ(e->v2, v1);
		ASSERT_EQ(1, v1->edges.size());
		ASSERT_EQ(1, v2->edges.size());

		// delete edge
		g->deleteEdge(e);
		ASSERT_EQ(0, v1->edges.size());
		ASSERT_EQ(0, v2->edges.size());
		ASSERT_EQ(0, g->getEdges().size());

		delete g;

	}

	TEST(Graph, Assign2) {

		Graph<Vertex, Edge>* g = new Graph<Vertex, Edge>();

		Vertex* v1 = g->createNewVertex();
		Vertex* v2 = g->createNewVertex();
		Vertex* v3 = g->createNewVertex();
		ASSERT_EQ(3, g->getVertices().size());

		Edge* e12 = g->createNewEdge(v1, v2);
		Edge* e23 = g->createNewEdge(v2, v3);
		Edge* e31 = g->createNewEdge(v3, v1);
		ASSERT_EQ(3, g->getEdges().size());

		ASSERT_EQ(2, v1->edges.size());
		ASSERT_EQ(2, v2->edges.size());
		ASSERT_EQ(2, v3->edges.size());

		g->deleteVertex(v1);
		ASSERT_EQ(1, v2->edges.size());
		ASSERT_EQ(1, v3->edges.size());
		ASSERT_EQ(2, g->getVertices().size());
		ASSERT_EQ(1, g->getEdges().size());

		ASSERT_EQ(v2, e23->getV1());
		ASSERT_EQ(v3, e23->getV2());

		delete g;

	}

	TEST(Graph, EdgePresent) {

		Graph<Vertex, Edge>* g = new Graph<Vertex, Edge>();
		Vertex* v1 = g->createNewVertex();
		Vertex* v2 = g->createNewVertex();

#warning should crash! already present

		g->createNewEdge(v1, v2);
		g->createNewEdge(v1, v2);

	}

}


#endif
