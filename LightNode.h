#ifndef SIGMA_LIGHTNODE
#define SIGMA_LIGHTNODE

#include "Node.h" // Base class: sig::Node
#include "Color.h"

namespace sig
{

	class LightNode : public Node
	{
	public:
		LightNode();
		
		void Update(float dt);
		
		int GetType() { return LIGHT; }
		
		LightNode* SetColor(const Color& color) {this->m_color = color; return this;}
		LightNode* SetZ(float z) {this->m_z = z; return this;}
		const Color& GetColor() const {return m_color;}
		float GetZ() const {return m_z;}
	private:
		static int index;
		float m_z;
		Color m_color;
	};

}

#endif // SIGMA_LIGHTNODE
