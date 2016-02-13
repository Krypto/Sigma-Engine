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

Matrix4 Matrix4::MakeRotation(Scalar& angle, Vector3 axis)
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

Matrix4 Matrix4::MakeOrtho(Scalar left, Scalar right, Scalar bottom, Scalar top, Scalar znear, Scalar zfar)
{
    float width = right - left;
    float height = bottom - top;

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

}}