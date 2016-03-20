#ifndef SIGMA_DRAWABLE_STATIC_IMG
#define SIGMA_DRAWABLE_STATIC_IMG

#include "Component.h"
#include "SIG_OpenGL.h"
#include "Rect.h"
#include "Color.h"

#include "Texture2D.h"
#include "Shader.h"
#include "sig_gpu_builtin.h"

#include <string>
using namespace std;

namespace sig
{
	class SpriteBatch;
	class Shader;
	class Sprite : public Component
	{
	public:
		Sprite();
		Sprite(Texture2D *tex);
		Sprite(Texture2D *tex, Shader *shad);
		~Sprite() {}
		
		Shader* GetShader() { return m_shader; }
		void SetShader(Shader* shader) { this->m_shader = shader; }

		Texture2D* GetTexture() { return m_texture; }
		void SetTexture(Texture2D* texture) { this->m_texture = texture; }

		const Rect& GetUVRectangle() const { return m_uvRectangle; }
		void SetUVRectangle(const Rect& uvRectangle) { this->m_uvRectangle = uvRectangle; }

		const Color& GetColor() const { return m_color; }
		void SetColor(const Color& color) { this->m_color = color; }

		int GetDrawOrder() const;
		void SetDrawOrder(int drawOrder);

		void Render(SpriteBatch *batch);
		
		COMPONENT_NAME("Sprite")
	protected:
		Texture2D *m_texture;
		Shader *m_shader;
		Rect m_uvRectangle;
		Color m_color;
		int m_drawOrder;
	};
}

#endif // SIGMA_DRAWABLE_STATIC_IMG
