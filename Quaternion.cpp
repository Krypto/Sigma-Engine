#include "Quaternion.h"

namespace sig { namespace math {
	
Quaternion::Quaternion(Matrix4 &mat) {
	float tr = mat(0, 0) + mat(1, 1) + mat(2, 2);

	if (tr > 0.0f) {
		float s = sqrtf(tr + 1.0f) * 2.0f;
		m_w = 0.25f * s;
		m_x = (mat(2, 1) - mat(1, 2)) / s;
		m_y = (mat(0, 2) - mat(2, 0)) / s;
		m_z = (mat(1, 0) - mat(0, 1)) / s;
	} else {
		if (mat(0, 0) > mat(1, 1) && mat(0, 0) > mat(2, 2)) {
			float s = sqrtf(1.0f + mat(0, 0) - mat(1, 1) - mat(2, 2)) * 2.0f;
			m_w = (mat(2, 1) - mat(1, 2)) / s;
			m_x = 0.25f * s;
			m_y = (mat(0, 1) + mat(1, 0)) / s;
			m_z = (mat(0, 2) + mat(2, 0)) / s;
		} else if (mat(1, 1) > mat(2, 2)) {
			float s = sqrtf(1.0f + mat(1, 1) - mat(0, 0) - mat(2, 2)) * 2.0f;
			m_w = (mat(0, 2) - mat(2, 0)) / s;
			m_x = (mat(0, 1) + mat(1, 0)) / s;
			m_y = 0.25f * s;
			m_z = (mat(1, 2) + mat(2, 1)) / s;
		} else {
			float s = sqrtf(1.0f + mat(2, 2) - mat(0, 0) - mat(1, 1)) * 2.0f;
			m_w = (mat(1, 0) - mat(0, 1)) / s;
			m_x = (mat(0, 2) + mat(2, 0)) / s;
			m_y = (mat(1, 2) + mat(2, 1)) / s;
			m_z = 0.25f * s;
		}
	}

	(*this) = Normalized();
}

Quaternion Quaternion::SetFromAxisAngle(Scalar angle, Vector3 axis) {
	float result = float(sin(angle / 2.0f));
	m_w = float(cos(angle / 2.0f));

	m_x = float(axis[0] * result);
	m_y = float(axis[1] * result);
	m_z = float(axis[2] * result);

	return (*this);
}

Quaternion Quaternion::SetFromEulerAngles(Vector3 &ea) {
	float c1 = cosf(ea[2] * 0.5f);
	float c2 = cosf(ea[1] * 0.5f);
	float c3 = cosf(ea[0] * 0.5f);
	float s1 = sinf(ea[2] * 0.5f);
	float s2 = sinf(ea[1] * 0.5f);
	float s3 = sinf(ea[0] * 0.5f);

	m_x = c1*c2*s3 - s1*s2*c3;
	m_y = c1*s2*c3 + s1*c2*s3;
	m_z = s1*c2*c3 - c1*s2*s3;
	m_w = c1*c2*c3 + s1*s2*s3;

	(*this) = Normalized();

	return (*this);
}

Quaternion Quaternion::operator *(const Quaternion &q) {
	Quaternion r;

	r.m_w = m_w*q.m_w - m_x*q.m_x - m_y*q.m_y - m_z*q.m_z;
	r.m_x = m_w*q.m_x + m_x*q.m_w + m_y*q.m_z - m_z*q.m_y;
	r.m_y = m_w*q.m_y + m_y*q.m_w + m_z*q.m_x - m_x*q.m_z;
	r.m_z = m_w*q.m_z + m_z*q.m_w + m_x*q.m_y - m_y*q.m_x;

	return r;
}

Quaternion Quaternion::Normalized() {
	float len = Length();
	float l_x = m_x / len;
	float l_y = m_y / len;
	float l_z = m_z / len;
	float l_w = m_w / len;

	return Quaternion(l_x, l_y, l_z, l_w);
}

Scalar Quaternion::Length() const {
	return sqrtf(m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w);
}

Vector3 Quaternion::ToEuler() const {
	float q0 = m_w;
	float q1 = m_y;
	float q2 = m_x;
	float q3 = m_z;

	Vector3 radAngles;
	radAngles.SetY(atan2f(2.0f * (q0 * q1 + q2 * q3), 1.0f - 2.0f * (powf(q1, 2.0f) + powf(q2, 2.0f))));
	radAngles.SetX(asinf(2.0f * (q0 * q2 - q3 * q1)));
	radAngles.SetZ(atan2f(2.0f * (q0 * q3 + q1 * q2), 1.0f - 2.0f * (powf(q2, 2.0f) + powf(q3, 2.0f))));

	return radAngles;
}

}}