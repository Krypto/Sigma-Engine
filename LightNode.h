#ifndef SIGMA_LIGHTNODE
#define SIGMA_LIGHTNODE

#include "Component.h"
#include "Color.h"

namespace sig
{

	class Light : public Component
	{
	public:
		Light();
		
		void Update(float dt);

		Color& GetColor() { return m_color; }
		void SetColor(const Color& color) { this->m_color = color; }

		float& GetZ() { return m_z; }
		void SetZ(float z) { this->m_z = z; }

	private:
		static int index;
		float m_z;
		Color m_color;
	};

}

#endif // SIGMA_LIGHTNODE
