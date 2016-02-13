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
		
		Sprite* SetShader(Shader* shader) {this->m_shader = shader; return this;}
		Sprite* SetTexture(Texture2D* texture) {this->m_texture = texture; return this;}
		Sprite* SetUVRectangle(const Rect& uvRectangle) {this->m_uvRectangle = uvRectangle; return this;}
		Shader* GetShader() {return m_shader;}
		Texture2D* GetTexture() {return m_texture;}
		const Rect& GetUVRectangle() const {return m_uvRectangle;}
		
		Sprite* SetColor(const Color& color) {this->m_color = color; return this;}
		const Color& GetColor() const {return m_color;}
		
		void Render(SpriteBatch *batch);
		
		virtual string GetName() const { return "Sprite"; }
	protected:
		Texture2D *m_texture;
		Shader *m_shader;
		Rect m_uvRectangle;
		Color m_color;
	};
}

#endif // SIGMA_DRAWABLE_STATIC_IMG
