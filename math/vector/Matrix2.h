#ifndef K_MATRIX2_H
#define K_MATRIX2_H

namespace K {

	// layout:
	// A B
	// C D

	struct Eigenvalues2 {
		float e1, e2;
		void normalize() {		// if you are only interested in the ratio between e1 and e2
			float len = std::sqrt(e1*e1 + e2*e2);
			e1 /= len;
			e2 /= len;
		}
	};

	struct Eigenvector2 {
		float x, y;
		void normalize() {
			float len = std::sqrt(x*x + y*y);
			x /= len;
			y /= len;
		}
	};

	struct Eigenvectors2 {Eigenvector2 v1, v2;};

	struct EigenResult2 {Eigenvalues2 eVal; Eigenvectors2 eVec;};

	/** helper-class to calculate the eigenvalues of a 2x2 matrix */
	struct Matrix2 {

		float a,b,c,d;

		Matrix2() : a(0), b(0), c(0), d(0) {;}

		Matrix2(const float a, const float b, const float c, const float d) : a(a), b(b), c(c), d(d) {;}

		EigenResult2 getEigenvalues() const {
			const float T = a+d;
			const float D = a*d - b*c;

			EigenResult2 res;

			res.eVal.e1 = T/2 + std::sqrt(T*T/4-D);
			res.eVal.e2 = T/2 - std::sqrt(T*T/4-D);

			res.eVec.v1.x = b;
			res.eVec.v1.y = res.eVal.e1 - a;
			res.eVec.v1.normalize();

			res.eVec.v2.x = b;
			res.eVec.v2.y = res.eVal.e2 - a;
			res.eVec.v2.normalize();

			return res;

		}

		void operator += (const Matrix2& mat) {
			a += mat.a;
			b += mat.b;
			c += mat.c;
			d += mat.d;
		}

		void operator /= (const float v) {
			a /= v;
			b /= v;
			c /= v;
			d /= v;
		}



	};

}

#endif // K_MATRIX2_H
