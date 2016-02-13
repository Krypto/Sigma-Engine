#ifndef AE_QUAT
#define AE_QUAT

#include "Matrix4.h"

namespace sig { namespace math {
	
	class Quaternion {
	public:
		Quaternion()
			: Quaternion(0.0f, 0.0f, 0.0f, 1.0f)
		{}
		Quaternion(Matrix4 &mat);
		Quaternion(Scalar x, Scalar y, Scalar z, Scalar w)
			: m_x(x), m_y(y), m_z(z), m_w(w)
		{}

		Scalar X() const { return m_x; }
		Scalar Y() const { return m_y; }
		Scalar Z() const { return m_z; }
		Scalar W() const { return m_w; }
		void SetX(Scalar v) { m_x = v; }
		void SetY(Scalar v) { m_y = v; }
		void SetZ(Scalar v) { m_z = v; }
		void SetW(Scalar v) { m_w = v; }

		Quaternion SetFromAxisAngle(Scalar angle, Vector3 axis);
		Quaternion SetFromEulerAngles(Vector3 &ea);

		Quaternion Conjugate() const {
			return Quaternion(-m_x, -m_y, -m_z, m_w);
		}
		Scalar Length() const;
		Quaternion Normalized();

		Vector3 ToEuler() const;

		Quaternion operator *(const Quaternion &q);
	private:
		Scalar m_x, m_y, m_z, m_w;
	};
}}

#endif