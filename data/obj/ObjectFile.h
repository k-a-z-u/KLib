#ifndef K_DATA_OBJECTFILE_H
#define K_DATA_OBJECTFILE_H

#include <vector>
#include <fstream>

#include "../Tokenizer.h"

namespace K {

	/**
	 * prase .obj files
	 */
	class ObjFileReader {

	public:

		struct Vec2 {
			float x;
			float y;
			Vec2() {;}
			Vec2(const float x, const float y) : x(x), y(y) {;}
		};

		struct Vec3 {
			float x;
			float y;
			float z;
			Vec3() {;}
			Vec3(const float x, const float y, const float z) : x(x), y(y), z(z) {;}
			bool operator == (const Vec3& o) const {return x==o.x && y==o.y && z==o.z;}
		};

		/** group vertex+normal+texture */
		struct VNT {
			int idxVertex;
			int idxNormal;
			int idxTexture;
			Vec3 vertex;
			Vec3 normal;
			Vec2 texture;
		};

		/** one triangle */
		struct Face {
			VNT vnt[3];
			Face(VNT v1, VNT v2, VNT v3) : vnt({v1,v2,v3}) {;}
		};

		/** internal data */
		struct Data {
			std::vector<Vec3> vertices;
			std::vector<Vec2> texCoords;
			std::vector<Vec3> normals;
			std::vector<Face> faces;
		} data;

	public:

		/** ctor with the file to load */
		ObjFileReader(const std::string& file) {

			std::ifstream is(file);
			std::string line;
			while(getline(is, line)) {parseLine(line);}
			is.close();

		}

		/** get the parsed data */
		const Data& getData() const {return data;}


	private:

		/** parse one line of the .obj file */
		void parseLine(const std::string& line) {

			if (line.length() < 2) {return;}

			Tokenizer t(line, '\r');
			std::string token = t.getToken(' ');

			if ("v"  == token) {parseVertex(t);}
			if ("vt" == token) {parseTexCoord(t);}
			if ("vn" == token) {parseNormal(t);}
			if ("f"  == token) {parseFace(t);}

		}

		/** parse one vertex from the tokenizer */
		void parseVertex(Tokenizer& t) {
			const float x = std::stof(t.getToken(' '));
			const float y = std::stof(t.getToken(' '));
			const float z = std::stof(t.getToken(' '));
			data.vertices.push_back(Vec3(x,y,z));
		}

		/** parse one texture-coordinate from the tokenizer */
		void parseTexCoord(Tokenizer& t) {
			const float u = std::stof(t.getToken(' '));
			const float v = std::stof(t.getToken(' '));
			data.texCoords.push_back(Vec2(u, -v));
		}

		/** parse one normal from the tokenizer */
		void parseNormal(Tokenizer& t) {
			const float x = std::stof(t.getToken(' '));
			const float y = std::stof(t.getToken(' '));
			const float z = std::stof(t.getToken(' '));
			data.normals.push_back(Vec3(x,y,z));
		}

		/** parse one face from the tokenizer */
		void parseFace(Tokenizer& t) {

			std::vector<VNT> indices;

			int numVertices = 0;
			while(t.hasNext()) {

				++numVertices;
				const std::string token = t.getToken(' ');
				Tokenizer t2(token);
				const std::string v = t2.getToken('/', false);
				const std::string vt = t2.getToken('/', false);
				const std::string vn = t2.getToken('/', false);

				// create a new vertex/normal/texture combination
				VNT vnt;
				vnt.idxVertex =								(std::stoi(v)  - 1);
				vnt.idxNormal =		(vn.empty()) ? (-1) :	(std::stoi(vn) - 1);
				vnt.idxTexture =	(vt.empty()) ? (-1) :	(std::stoi(vt) - 1);

				if (vnt.idxVertex >= 0)		{vnt.vertex =	data.vertices[vnt.idxVertex];}
				if (vnt.idxNormal >= 0)		{vnt.normal =	data.normals[vnt.idxNormal];}
				if (vnt.idxTexture >= 0)	{vnt.texture =	data.texCoords[vnt.idxTexture];}

				indices.push_back(vnt);

			}

			// this will both, create normal triangles and triangulate polygons
			// see: http://www.mathopenref.com/polygontriangles.html
			for (int i = 1; i < (int) indices.size()-1; ++i) {
				Face face(indices[0], indices[1], indices[i+1]);
				data.faces.push_back(face);
			}

			// sanity check
			if (numVertices != 3) {throw "this face is not a triangle!";}

		}

	};


}

#endif // K_DATA_OBJECTFILE_H
