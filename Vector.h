#ifndef AE_VECTOR
#define AE_VECTOR

#include <assert.h>
#include <cmath>

using namespace std;

namespace sig { namespace math {
	
typedef float Scalar;
typedef unsigned int uint;

class Vector2 {
public:
	Vector2();
	Vector2(Scalar v);
	Vector2(Scalar x, Scalar y);

	Scalar X() const { return m_x; }
	Scalar Y() const { return m_y; }
	void SetX(Scalar v) { m_x = v; }
	void SetY(Scalar v) { m_y = v; }
	
	Scalar Length() const;
	Vector2 Normalized();
	Vector2 Max(const Vector2 &o) const;
	Scalar Max();

	Vector2 operator *(Scalar scalar) const;
	Vector2 operator /(Scalar scalar) const;
	Vector2 operator +(const Vector2 &other) const;
	Vector2 operator -(const Vector2 &other) const;

	bool operator ==(const Vector2 &other) const;
	bool operator !=(const Vector2 &other) const;

	Scalar operator [](uint i) {
		switch (i) {
		case 0:
			return m_x;
		case 1:
			return m_y;
		}
		return -1.0f;
	}

	Scalar operator [](uint i) const {
		switch (i) {
		case 0:
			return m_x;
		case 1:
			return m_y;
		}
		return -1.0f;
	}

	Scalar Cross(const Vector2 &other) const;
	Scalar Dot(const Vector2 &other) const;

	static const Vector2 Zero;
private:
	Scalar m_x, m_y;
};

class Vector3 {
public:
	Vector3();
	Vector3(Scalar v);
	Vector3(Scalar x, Scalar y, Scalar z);

	Scalar X() const { return m_x; }
	Scalar Y() const { return m_y; }
	Scalar Z() const { return m_z; }
	void SetX(Scalar v) { m_x = v; }
	void SetY(Scalar v) { m_y = v; }
	void SetZ(Scalar v) { m_z = v; }

	Scalar Length() const;
	Vector3 Normalized();

	Vector3 operator *(Scalar scalar) const;
	Vector3 operator /(Scalar scalar) const;
	Vector3 operator +(const Vector3 &other) const;
	Vector3 operator -(const Vector3 &other) const;
	Vector3 operator -() const;

	bool operator ==(const Vector3 &other) const;
	bool operator !=(const Vector3 &other) const;

	Scalar operator [](uint i) {
		switch (i) {
		case 0:
			return m_x;
		case 1:
			return m_y;
		case 2:
			return m_z;
		}
		return -1.0f;
	}

	Scalar operator [](uint i) const {
		switch (i) {
		case 0:
			return m_x;
		case 1:
			return m_y;
		case 2:
			return m_z;
		}
		return -1.0f;
	}

	Vector3 Cross(const Vector3 &other) const;
	Scalar Dot(const Vector3 &other) const;

	static const Vector3 Zero;
private:
	Scalar m_x, m_y, m_z;
};

class Vector4 {
public:
	Vector4();
	Vector4(Scalar v);
	Vector4(Scalar x, Scalar y, Scalar z, Scalar w);

	Scalar X() const { return m_x; }
	Scalar Y() const { return m_y; }
	Scalar Z() const { return m_z; }
	Scalar W() const { return m_w; }
	void SetX(Scalar v) { m_x = v; }
	void SetY(Scalar v) { m_y = v; }
	void SetZ(Scalar v) { m_z = v; }
	void SetW(Scalar v) { m_w = v; }

	Scalar Length() const;
	Vector4 Normalized();

	Vector4 operator *(Scalar scalar) const;
	Vector4 operator /(Scalar scalar) const;
	Vector4 operator +(const Vector4 &other) const;
	Vector4 operator -(const Vector4 &other) const;
	Vector4 operator -() const;

	bool operator ==(const Vector4 &other) const;
	bool operator !=(const Vector4 &other) const;

	Scalar operator [](uint i) {
		switch (i) {
		case 0:
			return m_x;
		case 1:
			return m_y;
		case 2:
			return m_z;
		case 3:
			return m_w;
		}
		return -1.0f;
	}

	Scalar operator [](uint i) const {
		switch (i) {
		case 0:
			return m_x;
		case 1:
			return m_y;
		case 2:
			return m_z;
		case 3:
			return m_w;
		}
		return -1.0f;
	}

private:
	Scalar m_x, m_y, m_z, m_w;
};

}}
#endif
