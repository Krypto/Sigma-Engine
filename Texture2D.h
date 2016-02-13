#ifndef SIGMA_TEXTURE_2D
#define SIGMA_TEXTURE_2D

#include "SIG_Utilities.h"
#include "SIG_OpenGL.h"

#include <string>
using namespace std;

namespace sig
{

	class Texture2D
	{
	public:
		enum {
			NO_ATTACHMENT,
			DEPTH_ATTACHMENT = GL_DEPTH_ATTACHMENT,
			STENCIL_ATTACHMENT = GL_STENCIL_ATTACHMENT,
			COLOR_ATTACHMENT = GL_COLOR_ATTACHMENT0
		};
		
		Texture2D();
		Texture2D(int w, int h, int attachment);
		Texture2D(Byte* data, int w, int h);
		Texture2D(const string &fileName);
		
		/**
		 * @brief Set the texture filter
		 * @param filter GL_NEAREST, GL_LINEAR, etc...
		 */
		void SetFilter(int filter);
		
		/**
		 * @brief Bind texture for rendering
		 * @param slot Texture slot to activate  0 <= slot < 32
		 */
		void Bind(int slot=0);
		
		/**
		 * @brief Bing this texture in write mode for rendering
		 */
		void BindForRendering();
		
		/**
		 * @brief Unbind the current texture
		 */
		static void Unbind();
				
		void Draw(float x, float y, float w, float h);
				
		unsigned int GetBindCode() const { return m_bindCode; }
		int GetHeight() const { return m_height; }
		int GetWidth() const { return m_width; }
		
		~Texture2D();
	private:
		int m_width, m_height;
		unsigned int m_bindCode;
		unsigned int m_fboBindCode;
		
		void Init(int w, int h, Byte* data, int format, int filter, bool clamp, int attach);
		void InitFBO(int attachment);
	};

}

#endif // SIGMA_TEXTURE_2D
