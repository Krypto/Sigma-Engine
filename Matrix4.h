#ifndef AE_MATRIX4
#define AE_MATRIX4

#include "Vector.h"

namespace sig { namespace math {
	class Quaternion;
	class Matrix4 {
	public:
		Matrix4();
		Matrix4(Quaternion q);
		Matrix4(Scalar m[16]);

		Scalar& operator() (uint row, uint col);
		Scalar operator() (uint row, uint col) const;

		Matrix4 Inverted();
		Matrix4 Transposed();
		Matrix4 RotationFromVectors(Vector3 forward, Vector3 up, Vector3 right);

		Vector3 TransformPoint(const Vector3& pt);

		const static Matrix4 Identity;

		static Matrix4 MakeTranslation(const Vector3 &trans);
		static Matrix4 MakeRotation(Scalar angle, Vector3 axis);
		static Matrix4 MakeScale(const Vector3 &scal);
		static Matrix4 MakeLookAt(const Vector3 &eye, const Vector3 &center, const Vector3 &up);
		static Matrix4 MakePerspective(Scalar fov, Scalar aspect, Scalar znear, Scalar zfar);
		static Matrix4 MakeOrtho(Scalar width, Scalar height, Scalar znear, Scalar zfar);

		Scalar** Raw() {
			Scalar **rows = new Scalar*[4] { m_matrix[0], m_matrix[1], m_matrix[2], m_matrix[3] };
			return rows;
		}

		Scalar Get(int r, int c) const {
			return m_matrix[r][c];
		}
		void Set(int r, int c, Scalar v) {
			m_matrix[r][c] = v;
		}

		const Vector4 &operator *(const Vector4& v);
		const Matrix4 operator *(const Matrix4 &m) const;

	private:
		Scalar m_matrix[4][4];
	};

}}

#endif
