#ifndef XYZFILE_H
#define XYZFILE_H

#include <string>
#include <fstream>

namespace K {

	/** read xyz point-clouds */
	class XYZFileReader {

	public:

		struct Vec3 {
			float x;
			float y;
			float z;
			Vec3() {;}
			Vec3(const float x, const float y, const float z) : x(x), y(y), z(z) {;}
		};

		struct Data {
			std::vector<Vec3> vertices;
			std::vector<Vec3> normals;
		} data;

	private:

		bool swapXY;

	public:

		/** ctor with the file to load */
		XYZFileReader(const std::string& file, const bool swapXY = false) : swapXY(swapXY) {

			std::ifstream is(file);
			std::string line;
			while(getline(is, line)) {parseLine(line);}
			is.close();

		}

		/** get the internal data structure */
		const Data& getData() const {return data;}

	private:

		void parseLine(const std::string& line) {

			// store temporal values. uninitialized!
			double tmp[32];

			// read all space/tab-separated float values
			int cnt = 0;
			size_t start = 0;

			for (int i = 0; i < (int) line.length(); ++i) {

				// next char
				const char c = line[i];

				// what to do
				switch (c) {
					case ' ':
					case '\t':
					case '\n':
					case '\r':
						const size_t end = i;
						const size_t len = end-start;
						if (len > 0) {
							const std::string number = line.substr(start, end-start);
							tmp[cnt] = std::stod(number);
							start = end+1;
							++cnt;
						}
				}

				if (c == '\r') {break;}
				if (c == '\n') {break;}

			}

			// vertices available?
			if (cnt >= 3) {
				if (swapXY) {
					data.vertices.push_back(Vec3(tmp[0], tmp[2], tmp[1]));
				} else {
					data.vertices.push_back(Vec3(tmp[0], tmp[1], tmp[2]));
				}
			}

			// normals available?
			if (cnt >= 6) {
				data.normals.push_back(Vec3(tmp[3], tmp[4], tmp[5]));
			}

		}

	};

}

#endif // XYZFILE_H
