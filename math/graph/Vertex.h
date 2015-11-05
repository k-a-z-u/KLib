#ifndef K_MATH_GRAPH_VERTEX_H
#define K_MATH_GRAPH_VERTEX_H

#include <vector>
#include <algorithm>


namespace K {

	class Edge;

	/**
	 * template base for all vertices.
	 * a vertex contains a vector of attached edges.
	 */
	class Vertex {

		template <typename Vertex, typename Edge> friend class Graph;
		friend class Graph_Assign_Test;
		friend class Graph_Assign2_Test;


	private:

		/** all edges from this vertex to other vertices */
		std::vector<Edge*> edges;


	protected:

		/** hidden empty ctor */
		Vertex() {;}


	public:

		/** dtor */
		virtual ~Vertex() {
			edges.clear();
		}

		/** get an immutable vector of all edges */
		const std::vector<Edge*>& getEdges() {
			return edges;
		}

		/** check whether this vertex has the given edge */
		bool hasEdge(Edge* edge) {
			for (const Edge* e : edges) {
				if (e == edge) {return true;}
			}
			return false;
		}

		/** get all neighbors for this vertex */
		inline std::vector<Vertex*> getNeighbors() const;

		/** is this vertex connected to the given vertex? */
		inline bool isConnectedTo(Vertex* v) const;

	protected:

		friend class Edge;

		/** add a new edge to this vertex */
		void addEdge(Edge* edge) {
			edges.push_back(edge);
		}

		/** remove the given edge from this vertex */
		inline void removeEdge(Edge* edge);

		/** remove all edges from this vertex */
		inline void removeAllEdges();

	};

}

#include "Edge.h"

void K::Vertex::removeEdge(Edge* edge) {
	this->edges.erase(std::remove(this->edges.begin(), this->edges.end(), edge), this->edges.end());
}

void K::Vertex::removeAllEdges() {
//	for (Edge* edge : edges) {
//		Vertex* other = edge->getOther(this);
//		other->edges.erase(std::remove(other->edges.begin(), other->edges.end(), edge), other->edges.end());
//	}
	edges.clear();
}

std::vector<K::Vertex*> K::Vertex::getNeighbors() const {
	std::vector<K::Vertex*> neighbors;
	for(const Edge* edge : edges) {
		K::Vertex* o = edge->getOther(this);
		neighbors.push_back(o);
	}
	return neighbors;
}

bool K::Vertex::isConnectedTo(Vertex* v) const {
	for (const Edge* edge : edges) {
		if (edge->contains(v)) {return true;}
	}
	return false;
}


#endif // K_MATH_GRAPH_VERTEX_H
