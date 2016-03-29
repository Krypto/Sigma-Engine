#ifndef SIGMA_CAMERA
#define SIGMA_CAMERA

#include "Node.h"
#include "Matrix4.h"

namespace sig
{

	class Window;
	class Camera2D : public Node
	{
	public:
		Camera2D();
		
		void ApplyTransformation(Window* w);
	
		float& GetZoom() { return m_zoom; }
		void SetZoom(float zoom) {
			if (zoom >= 0.01f) {
				this->m_zoom = zoom;
			}
		}

		math::Matrix4 GetCameraMatrix();
	private:
		float m_zoom;
	};

}

#endif // SIGMA_CAMERA
