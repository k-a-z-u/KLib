#ifndef K_MATH_GRAPH_EDGE_H
#define K_MATH_GRAPH_EDGE_H

	class Vertex;

namespace K {

	/**
	 * describes on edge by using two vertices
	 */
	class Edge {

		friend class Vertex;
		template <typename Vertex, typename Edge> friend class Graph;
		friend class Graph_Assign_Test;
		friend class Graph_Assign2_Test;

	protected:

		/** the first vertex of this edge */
		Vertex* v1;

		/** the second vertex of this edge */
		Vertex* v2;


	protected:

		/** hidden empty ctor */
		Edge() : v1(nullptr), v2(nullptr) {
			;
		}

		/** hidden ctor */
		Edge(Vertex* v1, Vertex* v2) : v1(nullptr), v2(nullptr) {
			setV1(v1);
			setV2(v2);
		}


	public:


		/** dtor */
		virtual ~Edge() {

			// ensure we are unregistered
			setV1(nullptr);
			setV2(nullptr);

		}


		/** set the first vertex */
		inline void setV1(Vertex* _v1);

		/** set the second vertex */
		inline void setV2(Vertex* _v2);


		/** get the first vertex */
		Vertex* getV1() const {return v1;}

		/** get the second vertex */
		Vertex* getV2() const {return v2;}


		/** return the other end of the edge that is not v1 */
		Vertex* getOther(const Vertex* v) const { return (v == v1) ? (v2) : (v1); }

		/** check whether both instances match */
		//bool operator == (const Vertex* other) { return (void*)other == (void*)this; }

		/** check whether this edge is similar to another edge */
		bool operator == (const Edge& e) const {
			return (e.v1 == v1) && (e.v2 == v2);
		}


		/** does this edge contain the given vector? */
		bool contains(const Vertex* v) const {return v1 == v || v2 == v;}



	protected:

		/** disconnect this edge from the given vertex (both ways) */
		inline void disconnect(Vertex* v);

		/** disconnect this edge from both endes */
		inline void disconnect();

	};

}

#include "Vertex.h"

void K::Edge::disconnect() {
	if (v1 != nullptr) {setV1(nullptr);}
	if (v2 != nullptr) {setV2(nullptr);}
}

void K::Edge::disconnect(Vertex* v) {
	if (v1 == v) {setV1(nullptr);}
	if (v2 == v) {setV2(nullptr);}
}

void K::Edge::setV1(Vertex* _v1) {
	if (v1) {v1->removeEdge(this);}					// unregister from previous node
	v1 = _v1;
	if (v1) {v1->addEdge(this);}					// register to the new node
}

void K::Edge::setV2(Vertex* _v2) {
	if (this->v2) {v2->removeEdge(this);}			// unregister from previous node
	v2 = _v2;
	if (v2) {v2->addEdge(this);}					// register to the new node
}



#endif // K_MATH_GRAPH_EDGE_H
