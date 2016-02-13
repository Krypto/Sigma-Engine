#include "Vector.h"

namespace sig { namespace math {
/// VECTOR 2
const Vector2 Vector2::Zero = Vector2(0.0f);
Vector2::Vector2() 
	: Vector2(0.0f)
{

}
Vector2::Vector2(Scalar v)
	: Vector2(v, v)
{

}
Vector2::Vector2(Scalar x, Scalar y) {
	this->m_x = x;
	this->m_y = y;
}

float Vector2::Length() const {
	return sqrtf(m_x * m_x + m_y * m_y);
}

Vector2 Vector2::Normalized() {
	float len = Length();
	float l_x = m_x / len;
	float l_y = m_y / len;

	return Vector2(l_x, l_y);
}

Vector2 Vector2::Max(const Vector2& o) const
{
	return Vector2(	o.X() > X() ? o.X() : X(),
					o.Y() > Y() ? o.Y() : Y());
}

Scalar sig::math::Vector2::Max()
{
	return X() > Y() ? X() : Y();
}

Vector2 Vector2::operator *(Scalar scalar) const {
	return Vector2(m_x * scalar, m_y * scalar);
}

Vector2 Vector2::operator /(Scalar scalar) const {
	assert(scalar > 0.0f);
	return Vector2(m_x / scalar, m_y / scalar);
}

Vector2 Vector2::operator +(const Vector2 &other) const {
	return Vector2(m_x + other.m_x, m_y + other.m_y);
}

Vector2 Vector2::operator -(const Vector2 &other) const {
	return Vector2(m_x - other.m_x, m_y - other.m_y);
}

Vector2 Vector2::operator -() const {
	return (*this) * -1.0f;
}

bool Vector2::operator ==(const Vector2 &other) const {
	return m_x == other.m_x && m_y == other.m_y;
}

bool Vector2::operator !=(const Vector2 &other) const {
	return m_x != other.m_x || m_y != other.m_y;
}

Scalar Vector2::Cross(const Vector2 &other) const {
	return (m_x * other.m_y) - (m_y * other.m_x);
}

Scalar Vector2::Dot(const Vector2 &other) const {
	return (m_x * other.m_x) + (m_y * other.m_y);
}

/// VECTOR 3
const Vector3 Vector3::Zero = Vector3(0.0f);
Vector3::Vector3()
	: Vector3(0.0f)
{

}
Vector3::Vector3(Scalar v)
	: Vector3(v, v, v)
{

}
Vector3::Vector3(Scalar x, Scalar y, Scalar z) {
	m_x = x;
	m_y = y;
	m_z = z;
}

Scalar Vector3::Length() const {
	return sqrtf(m_x * m_x + m_y * m_y + m_z * m_z);
}

Vector3 Vector3::Normalized() {
	float len = Length();
	float l_x = m_x / len;
	float l_y = m_y / len;
	float l_z = m_z / len;

	return Vector3(l_x, l_y, l_z);
}

Vector3 Vector3::operator *(Scalar scalar) const {
	return Vector3(m_x * scalar, m_y * scalar, m_z * scalar);
}

Vector3 Vector3::operator /(Scalar scalar) const {
	assert(scalar > 0.0f);
	return Vector3(m_x / scalar, m_y / scalar, m_z / scalar);
}

Vector3 Vector3::operator +(const Vector3 &other) const {
	return Vector3(m_x + other.m_x, m_y + other.m_y, m_z + other.m_z);
}

Vector3 Vector3::operator -(const Vector3 &other) const {
	return Vector3(m_x - other.m_x, m_y - other.m_y, m_z - other.m_z);
}

Vector3 Vector3::operator -() const {
	return (*this) * -1.0f;
}

bool Vector3::operator ==(const Vector3 &other) const {
	return m_x == other.m_x && m_y == other.m_y && m_z == other.m_z;
}
bool Vector3::operator !=(const Vector3 &other) const {
	return m_x != other.m_x || m_y != other.m_y || m_z != other.m_z;
}

Vector3 Vector3::Cross(const Vector3 &other) const {
	return Vector3(
		m_y * other.m_z - m_z * other.m_y,
		m_z * other.m_x - m_x * other.m_z,
		m_x * other.m_y - m_y * other.m_x);
}

Scalar Vector3::Dot(const Vector3 &other) const {
	return m_x * other.m_x + m_y * other.m_y + m_z * other.m_z;
}

/// VECTOR 4

Vector4::Vector4()
	: Vector4(0.0f)
{

}
Vector4::Vector4(Scalar v)
	: Vector4(v, v, v, v)
{

}
Vector4::Vector4(Scalar x, Scalar y, Scalar z, Scalar w) {
	m_x = x;
	m_y = y;
	m_z = z;
	m_w = w;
}

Scalar Vector4::Length() const {
	return sqrtf(m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w);
}

Vector4 Vector4::Normalized() {
	float len = Length();
	float l_x = m_x / len;
	float l_y = m_y / len;
	float l_z = m_z / len;
	float l_w = m_w / len;

	return Vector4(l_x, l_y, l_z, l_w);
}

Vector4 Vector4::operator *(Scalar scalar) const {
	return Vector4(m_x * scalar, m_y * scalar, m_z * scalar, m_w * scalar);
}

Vector4 Vector4::operator /(Scalar scalar) const {
	assert(scalar > 0.0f);
	return Vector4(m_x / scalar, m_y / scalar, m_z / scalar, m_w / scalar);
}

Vector4 Vector4::operator +(const Vector4 &other) const {
	return Vector4(m_x + other.m_x, m_y + other.m_y, m_z + other.m_z, m_w * other.m_w);
}

Vector4 Vector4::operator -(const Vector4 &other) const {
	return Vector4(m_x - other.m_x, m_y - other.m_y, m_z - other.m_z, m_w - other.m_w);
}

Vector4 Vector4::operator -() const {
	return (*this) * -1.0f;
}

bool Vector4::operator ==(const Vector4 &other) const {
	return m_x == other.m_x && m_y == other.m_y && m_z == other.m_z && m_w == other.m_w;
}
bool Vector4::operator !=(const Vector4 &other) const {
	return m_x != other.m_x || m_y != other.m_y || m_z != other.m_z || m_w != other.m_w;
}

}}