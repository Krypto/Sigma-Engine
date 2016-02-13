#ifndef SIGMA_CAMERA
#define SIGMA_CAMERA

#include "Node.h"
#include "Matrix4.h"

namespace sig
{

	class Camera2D : public Node
	{
	public:
		Camera2D();
		
		void ApplyTransformation(int w, int h);
	
		Camera2D* SetZoom(float zoom) {
			if (zoom >= 1) {
				this->m_zoom = zoom;
			}
			return this;
		}
		float GetZoom() const {return m_zoom;}
	private:
		float m_zoom, m_zNear, m_zFar;
	};

}

#endif // SIGMA_CAMERA
