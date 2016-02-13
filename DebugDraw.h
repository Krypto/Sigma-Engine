#ifndef SIGMA_PHYSICS_DRAW
#define SIGMA_PHYSICS_DRAW

#include "SIG_OpenGL.h"
#include <Box2D/Box2D.h>

namespace sig
{

	class DebugDraw : public b2Draw
	{
	public:
		void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) 
		{
			glBindTexture(GL_TEXTURE_2D, 0);
			glUseProgram(0);
			
			float verts[16];
			glVertexPointer(2, GL_FLOAT, 0, verts);			
			glEnableClientState(GL_VERTEX_ARRAY);
			
			for (int i = 0; i < vertexCount; i++) {
				verts[i*2]		= vertices[i].x;
				verts[i*2+1]	= vertices[i].y;
			}
			
			glColor4f(color.r, color.g, color.b, 1);
			glDrawArrays(GL_LINE_LOOP, 0, vertexCount);
		}
		
		void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
		{
			glBindTexture(GL_TEXTURE_2D, 0);
			glUseProgram(0);
			
			float verts[16];
			glVertexPointer(2, GL_FLOAT, 0, verts);			
			glEnableClientState(GL_VERTEX_ARRAY);
			
			for (int i = 0; i < vertexCount; i++) {
				verts[i*2]		= vertices[i].x;
				verts[i*2+1]	= vertices[i].y;
			}
			
			glColor4f(color.r, color.g, color.b, 0.25f);
			glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
			
			glColor4f(color.r, color.g, color.b, 1);
			glDrawArrays(GL_LINE_LOOP, 0, vertexCount);
		}
		
		void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) {}
		void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) {}
		void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {}
		void DrawTransform(const b2Transform& xf) {}

	};

}

#endif // SIGMA_PHYSICS_DRAW
