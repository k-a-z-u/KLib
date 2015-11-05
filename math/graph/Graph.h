#ifndef K_MATH_GRAPH_GRAPH_H
#define K_MATH_GRAPH_GRAPH_H

#include <vector>

#include "Vertex.h"
#include "Edge.h"

namespace K {

	template <typename V, typename E> class Graph {

		friend class Graph_Assign_Test;

	public:

		/** ctor */
		Graph() {
			;
		}

		/** dtor */
		~Graph() {

			// delete all vertices and edges
			for (V* v : vertices)	{delete v;}
			for (E* e : edges)		{delete e;}

		}

		/** create a new, empty, vertex */
		V* createNewVertex() {
			V* v = new V();
			vertices.push_back(v);
			return v;
		}

		/** create a new, empty, edge */
		E* createNewEdge() {
			E* e = new E();
			edges.push_back(e);
			return e;
		}

		/** create a new edge between the two given vertices */
		E* createNewEdge(V* v1, V* v2) {
			E* e = new E(v1, v2);
			bool present = std::find(edges.begin(), edges.end(), e) != edges.end();
			if (present) {throw "this edge already exists!";}
			edges.push_back(e);
			return e;
		}

		/** delete the given edge */
		void deleteEdge(E* e) {
			edges.erase(std::remove(edges.begin(), edges.end(), e), edges.end());
			e->disconnect();
#warning FIX DELETE!
			//delete e;
		}

		/** delete the given vertex (and all its edges) */
		void deleteVertex (V* v) {
			vertices.erase(std::remove(vertices.begin(), vertices.end(), v), vertices.end());
			deleteEdges(v);
			delete v;
		}

		/** delete all edges from the given vertex */
		void deleteEdges(V* v) {
			std::vector<Edge*> tmp(v->edges);
			for (Edge* e : tmp) { deleteEdge( (E*) e ); }
		}

		/** delete all edges */
		void deleteEdges() {
			for (int i = 0; i < edges.size(); ++i) {
				edges[i]->disconnect();
			}
			edges.clear();
		}

		/** get vertex by ID */
		V* getVertex(unsigned int idx) {
			return vertices[idx];
		}

		/** get edge by ID */
		E* getEdge(unsigned int idx) {
			return edges[idx];
		}


		/** get immutable vector of all vertices */
		const std::vector<V*>& getVertices() const {return vertices;}

		/** get immutable vector of all edges */
		const std::vector<E*>& getEdges() const {return edges;}




	protected:

		/** all vertices within the graph */
		std::vector<V*> vertices;

		/** all edges within the graph */
		std::vector<E*> edges;


	};

}

#endif // K_MATH_GRAPH_GRAPH_H
