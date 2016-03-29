#include "Matrix4.h"
#include "Quaternion.h"

namespace sig { namespace math {

		const Matrix4 Matrix4::Identity = Matrix4(Quaternion(0.0f, 0.0f, 0.0f, 1.0f));

		Matrix4::Matrix4()
		{
			(*this) = Identity;
		}

		Matrix4::Matrix4(Quaternion q)
		{
			Vector3 forward = Vector3(2.0f * (q.X() * q.Z() - q.W() * q.Y()),
									  2.0f * (q.Y() * q.Z() + q.W() * q.X()),
									  1.0f - 2.0f * (q.X() * q.X() + q.Y() * q.Y()));
			Vector3 up = Vector3(2.0f * (q.X() * q.Y() + q.W() * q.Z()),
								 1.0f - 2.0f * (q.X() * q.X() + q.Z() * q.Z()),
								 2.0f * (q.Y() * q.Z() - q.W() * q.X()));
			Vector3 right = Vector3(1.0f - 2.0f * (q.Y() * q.Y() + q.Z() * q.Z()),
									2.0f * (q.X() * q.Y() - q.W() * q.Z()),
									2.0f * (q.X() * q.Z() + q.W() * q.Y()));

			RotationFromVectors(forward, up, right);
		}

		Matrix4 Matrix4::RotationFromVectors(Vector3 forward, Vector3 up, Vector3 right)
		{
			Vector3 n = forward;
			Vector3 v = up;
			Vector3 u = right;

			(*this)(0, 0) = u.X();
			(*this)(1, 0) = u.Y();
			(*this)(2, 0) = u.Z();
			(*this)(3, 0) = 0.0f;
			(*this)(0, 1) = v.X();
			(*this)(1, 1) = v.Y();
			(*this)(2, 1) = v.Z();
			(*this)(3, 1) = 0.0f;
			(*this)(0, 2) = n.X();
			(*this)(1, 2) = n.Y();
			(*this)(2, 2) = n.Z();
			(*this)(3, 2) = 0.0f;
			(*this)(0, 3) = 0.0f;
			(*this)(1, 3) = 0.0f;
			(*this)(2, 3) = 0.0f;
			(*this)(3, 3) = 1.0f;

			return *this;
		}

		Vector3 Matrix4::TransformPoint(const Vector3 &pt)
		{
			Matrix4 me = (*this);
			float w = pt.X() * me(0, 3) + pt.Y() * me(1, 3) + pt.Z() * me(2, 3) + me(3, 3);
			if (w) {
				const float iw = 1.0f / w;
				return Vector3(
							(pt.X() * me(0, 0) + pt.Y() * me(0, 1) + pt.Z() * me(0, 2) + me(0, 3)) * iw,
							(pt.X() * me(1, 0) + pt.Y() * me(1, 1) + pt.Z() * me(1, 2) + me(1, 3)) * iw,
							(pt.X() * me(2, 0) + pt.Y() * me(2, 1) + pt.Z() * me(2, 2) + me(2, 3)) * iw
							);
			} else {
				return Vector3::Zero;
			}
		}

		Matrix4::Matrix4(float m[16])
		{
			int index = 0;
			for(int i = 0; i < 4; i++) {
				for(int j = 0; j < 4; j++) {
					(*this)(i, j) = m[index++];
				}
			}
		}

		Scalar& Matrix4::operator()(uint row, uint col)
		{
			return m_matrix[row][col];
		}

		Scalar Matrix4::operator()(uint row, uint col) const
		{
			return m_matrix[row][col];
		}

		Matrix4 Matrix4::Inverted()
		{
			//
			// Inversion by Cramer's rule.  Code taken from an Intel publication
			//
			Scalar Result[4][4];
			Scalar tmp[12]; /* temp array for pairs */
			Scalar src[16]; /* array of transpose source matrix */
			Scalar det; /* determinant */
			/* transpose matrix */
			for (uint i = 0; i < 4; i++)
			{
				src[i + 0 ] = (*this)(i, 0);
				src[i + 4 ] = (*this)(i, 1);
				src[i + 8 ] = (*this)(i, 2);
				src[i + 12] = (*this)(i, 3);
			}
			/* calculate pairs for first 8 elements (cofactors) */
			tmp[0] = src[10] * src[15];
			tmp[1] = src[11] * src[14];
			tmp[2] = src[9] * src[15];
			tmp[3] = src[11] * src[13];
			tmp[4] = src[9] * src[14];
			tmp[5] = src[10] * src[13];
			tmp[6] = src[8] * src[15];
			tmp[7] = src[11] * src[12];
			tmp[8] = src[8] * src[14];
			tmp[9] = src[10] * src[12];
			tmp[10] = src[8] * src[13];
			tmp[11] = src[9] * src[12];
			/* calculate first 8 elements (cofactors) */
			Result[0][0] = tmp[0]*src[5] + tmp[3]*src[6] + tmp[4]*src[7];
			Result[0][0] -= tmp[1]*src[5] + tmp[2]*src[6] + tmp[5]*src[7];
			Result[0][1] = tmp[1]*src[4] + tmp[6]*src[6] + tmp[9]*src[7];
			Result[0][1] -= tmp[0]*src[4] + tmp[7]*src[6] + tmp[8]*src[7];
			Result[0][2] = tmp[2]*src[4] + tmp[7]*src[5] + tmp[10]*src[7];
			Result[0][2] -= tmp[3]*src[4] + tmp[6]*src[5] + tmp[11]*src[7];
			Result[0][3] = tmp[5]*src[4] + tmp[8]*src[5] + tmp[11]*src[6];
			Result[0][3] -= tmp[4]*src[4] + tmp[9]*src[5] + tmp[10]*src[6];
			Result[1][0] = tmp[1]*src[1] + tmp[2]*src[2] + tmp[5]*src[3];
			Result[1][0] -= tmp[0]*src[1] + tmp[3]*src[2] + tmp[4]*src[3];
			Result[1][1] = tmp[0]*src[0] + tmp[7]*src[2] + tmp[8]*src[3];
			Result[1][1] -= tmp[1]*src[0] + tmp[6]*src[2] + tmp[9]*src[3];
			Result[1][2] = tmp[3]*src[0] + tmp[6]*src[1] + tmp[11]*src[3];
			Result[1][2] -= tmp[2]*src[0] + tmp[7]*src[1] + tmp[10]*src[3];
			Result[1][3] = tmp[4]*src[0] + tmp[9]*src[1] + tmp[10]*src[2];
			Result[1][3] -= tmp[5]*src[0] + tmp[8]*src[1] + tmp[11]*src[2];
			/* calculate pairs for second 8 elements (cofactors) */
			tmp[0] = src[2]*src[7];
			tmp[1] = src[3]*src[6];
			tmp[2] = src[1]*src[7];
			tmp[3] = src[3]*src[5];
			tmp[4] = src[1]*src[6];
			tmp[5] = src[2]*src[5];

			tmp[6] = src[0]*src[7];
			tmp[7] = src[3]*src[4];
			tmp[8] = src[0]*src[6];
			tmp[9] = src[2]*src[4];
			tmp[10] = src[0]*src[5];
			tmp[11] = src[1]*src[4];
			/* calculate second 8 elements (cofactors) */
			Result[2][0] = tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15];
			Result[2][0] -= tmp[1]*src[13] + tmp[2]*src[14] + tmp[5]*src[15];
			Result[2][1] = tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15];
			Result[2][1] -= tmp[0]*src[12] + tmp[7]*src[14] + tmp[8]*src[15];
			Result[2][2] = tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15];
			Result[2][2] -= tmp[3]*src[12] + tmp[6]*src[13] + tmp[11]*src[15];
			Result[2][3] = tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14];
			Result[2][3] -= tmp[4]*src[12] + tmp[9]*src[13] + tmp[10]*src[14];
			Result[3][0] = tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9];
			Result[3][0] -= tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10];
			Result[3][1] = tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10];
			Result[3][1] -= tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8];
			Result[3][2] = tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8];
			Result[3][2] -= tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9];
			Result[3][3] = tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9];
			Result[3][3] -= tmp[8]*src[9] + tmp[11]*src[10] + tmp[5]*src[8];
			/* calculate determinant */
			det=src[0]*Result[0][0]+src[1]*Result[0][1]+src[2]*Result[0][2]+src[3]*Result[0][3];
			/* calculate matrix inverse */
			det = 1.0f / det;

			Matrix4 result;
			for (uint i = 0; i < 4; i++) {
				for (uint j = 0; j < 4; j++) {
					result(i, j) = (Result[i][j] * det);
				}
			}
			return result;
		}

		const Matrix4 Matrix4::operator*(const Matrix4& m) const
		{
			Matrix4 ret;
			Scalar sum;

			for(int i = 0; i < 4; i++) {
				for(int j = 0; j < 4; j++) {
					sum = 0;

					for(int k = 0; k < 4; ++k) {
						sum += (*this)(k, i) * m(j, k);
					}

					ret(j, i) = sum;
				}
			}

			return ret;
		}

		Matrix4 Matrix4::Transposed()
		{
			Matrix4 Result;
			for(uint i = 0; i < 4; i++) {
				for(uint j = 0; j < 4; j++) {
					Result(j, i) = (*this)(i, j);
				}
			}
			return Result;
		}

		Matrix4 Matrix4::MakeTranslation(const Vector3& trans)
		{
			Matrix4 translate;

			translate(3, 0) = trans[0];
			translate(3, 1) = trans[1];
			translate(3, 2) = trans[2];

			return translate;
		}

		Matrix4 Matrix4::MakeRotation(Scalar angle, Vector3 axis)
		{
			Matrix4 rotate;

			float s = sinf(angle);
			float c = cosf(angle);
			float t = 1.0f - c;

			Vector3 ax = axis / sqrtf((axis.Length() * axis.Length()));

			float x = ax[0];
			float y = ax[1];
			float z = ax[2];

			rotate(0, 0) = t * x * x + c;
			rotate(1, 0) = t * y * x + s * z;
			rotate(2, 0) = t * z * x - s * y;

			rotate(0, 1) = t * x * y - s * z;
			rotate(1, 1) = t * y * y + c;
			rotate(2, 1) = t * z * y + s * x;

			rotate(0, 2) = t * x * z + s * y;
			rotate(1, 2) = t * y * z - s * x;
			rotate(2, 2) = t * z * z + c;

			return rotate;
		}

		Matrix4 Matrix4::MakeScale(const Vector3& scal)
		{
			Matrix4 scale;

			scale(0, 0) = scal[0];
			scale(1, 1) = scal[1];
			scale(2, 2) = scal[2];

			return scale;
		}

		Matrix4 Matrix4::MakeLookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
		{
			Vector3 ax, ay, az;
			az = (eye - center).Normalized();
			ax = up.Cross(az).Normalized();
			ay = az.Cross(ax).Normalized();

			Matrix4 result;
			result(0, 0) = ax[0];
			result(1, 0) = ax[1];
			result(2, 0) = ax[2];
			result(3, 0) = -ax.Dot(eye);

			result(0, 1) = ay[0];
			result(1, 1) = ay[1];
			result(2, 1) = ay[2];
			result(3, 1) = -ay.Dot(eye);

			result(0, 2) = az[0];
			result(1, 2) = az[1];
			result(2, 2) = az[2];
			result(3, 2) = -az.Dot(eye);

			result(0, 3) = 0.0f;
			result(1, 3) = 0.0f;
			result(2, 3) = 0.0f;
			result(3, 3) = 1.0f;

			return result;
		}

		Matrix4 Matrix4::MakePerspective(Scalar fov, Scalar aspect, Scalar znear, Scalar zfar)
		{
			float width = 1.0f / tanf(fov / 2.0f);
			float height = aspect / tanf(fov / 2.0f);

			Matrix4 result;
			result(0, 0) = width;
			result(1, 0) = 0.0f;
			result(2, 0) = 0.0f;
			result(3, 0) = 0.0f;

			result(0, 1) = 0.0f;
			result(1, 1) = height;
			result(2, 1) = 0.0f;
			result(3, 1) = 0.0f;

			result(0, 2) = 0.0f;
			result(1, 2) = 0.0f;
			result(2, 2) = zfar / (znear - zfar);
			result(3, 2) = zfar * znear / (znear - zfar);

			result(0, 3) = 0.0f;
			result(1, 3) = 0.0f;
			result(2, 3) = -1.0f;
			result(3, 3) = 0.0f;

			return result;
		}

		Matrix4 Matrix4::MakeOrtho(Scalar width, Scalar height, Scalar znear, Scalar zfar)
		{
			Matrix4 result;
			result(0, 0) = 2.0f / width;
			result(1, 0) = 0.0f;
			result(2, 0) = 0.0f;
			result(3, 0) = 0.0f;

			result(0, 1) = 0.0f;
			result(1, 1) = 2.0f / height;
			result(2, 1) = 0.0f;
			result(3, 1) = 0.0f;

			result(0, 2) = 0.0f;
			result(1, 2) = 0.0f;
			result(2, 2) = 1.0f / (znear - zfar);
			result(3, 2) = znear / (znear - zfar);

			result(0, 3) = 0.0f;
			result(1, 3) = 0.0f;
			result(2, 3) = 0.0f;
			result(3, 3) = 1.0f;

			return result;
		}

		const Vector4 &Matrix4::operator *(const Vector4 &v)
		{
			Vector4 product;

			for (uint i = 0; i < 4; i++) {
				float value = 0.0;
				for (int j = 0; j < 4; j++) {
					value += (*this)(i, j) * v[j];
				}
				product[i] = value;
			}

			return product;
		}

	}}
