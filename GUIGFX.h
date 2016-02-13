#ifndef SIGMA_GUI_GFX
#define SIGMA_GUI_GFX

#include "Color.h"
#include "SIG_OpenGL.h"
#include "Texture2D.h"
#include "Vector.h"
#include "Rect.h"

#include <string>
#include <vector>
using namespace std;

namespace sig
{
	using namespace math;
	class Shader;
	class GFX
	{
	public:
		static void Initialize();
		static void SetFillColor(const Color &c);
		static void SetStrokeColor(const Color &c);
		static void SetStrokeWidth(float w);
		static void SetQuadUVs(float uvx1, float uvy1, float uvx2, float uvy2);
		static void DrawRectangle(const Rect &r, Texture2D* t = 0);
		static void DrawLine(const Vector2 &p1, const Vector2 &p2);
		static void DrawText(const string &text, const Vector2 &pos, Texture2D *bmpFont, float scale=1, float spacing=-10.0f);
		static Vector2 GetTextMetrics(const string &text, Texture2D *bmpFont, float scale=1, float spacing=-10.0f);
	private:
		static GLuint quadVBO, quadUV;
		static Shader *qs;
		
		static Color stroke;
		static Color fill;
		static float strokeWidth;
		static float *vertexArr;

		static void DrawQuadFIX(const Rect &co, const Rect&r);
	};
}

#endif // SIGMA_GUI_GFX
