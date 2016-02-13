#include "GUIGFX.h"
#include "Shader.h"
#include "SIG_Utilities.h"

sig::Color sig::GFX::stroke;
sig::Color sig::GFX::fill;
float sig::GFX::strokeWidth;
float *sig::GFX::vertexArr;
GLuint sig::GFX::quadVBO = 0;
GLuint sig::GFX::quadUV = 0;
sig::Shader* sig::GFX::qs = 0;

void sig::GFX::Initialize()
{
	fill = Color::WHITE;
	stroke = Color::BLACK;
	strokeWidth = 1;

	glGenBuffersARB(1, &quadVBO);

	float vertices[] = {
		0, 0, 0,
		0, 1, 0,
		1, 0, 0,
		1, 1, 0,
		1, 0, 0,
		0, 1, 0
	};
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, quadVBO);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(float) * 18, vertices, GL_STATIC_DRAW_ARB);

	glGenBuffers(1, &quadUV);
	SetQuadUVs(0, 0, 1, 1);

	string VertexShaderCode	= STR(
		  attribute vec3 vertexPosition;
		  attribute vec2 vertexUV;
		  varying vec2 uv;
		  void main()
		  {
		  	gl_Position = ftransform();
		  	uv = vertexUV;
		  }
	);
	
	string FragmentShaderCode = STR(
		  varying vec2 uv;
		  uniform int hasTex;
		  uniform vec4 color;
		  uniform sampler2D textureSampler;
		  void main()
		  {
			vec4 finalcolor = color;
			if (hasTex == 1) {
				finalcolor *= texture2D(textureSampler, uv);
			}
		  	gl_FragColor = finalcolor;
		  }
	);
	
	qs = Shader::FromString(VertexShaderCode, FragmentShaderCode);
}

void sig::GFX::SetFillColor(const Color& c)
{
	fill = c;
}

void sig::GFX::SetStrokeColor(const Color& c)
{
	stroke = c;
}

void sig::GFX::SetStrokeWidth(float w)
{
	strokeWidth = w;
}

void sig::GFX::DrawRectangle(const Rect& r, Texture2D* t)
{
	qs->Bind();

	qs->SetVector4("color", Vector4(fill.r, fill.g, fill.b, fill.a));
	
	if (t != nullptr) {
		t->Bind(0);	
		qs->SetInt("hasTex", 1);
		qs->SetInt("textureSampler", 0);
	} else {
		qs->SetInt("hasTex", 0);
	}
	
	glPushMatrix();
	
	glTranslatef(r.x, r.y, 0);
	glScalef(r.width, r.height, 1);
	
	glEnableVertexAttribArrayARB(0);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, quadVBO);
	glVertexAttribPointerARB(0, 3, GL_FLOAT, false, 0, 0);
	
	glEnableVertexAttribArrayARB(1);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, quadUV);
	glVertexAttribPointerARB(1, 2, GL_FLOAT, false, 0, 0);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	
	glPopMatrix();
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	Shader::Unbind();
}

void sig::GFX::DrawLine(const Vector2& p1, const Vector2& p2)
{
	glLineWidth(strokeWidth);
	glColor4f(stroke.r, stroke.g, stroke.b, stroke.a);
	glBegin(GL_LINES);
	glVertex2f(p1.X(), p1.Y());
	glVertex2f(p2.X(), p2.Y());
	glEnd();
	glLineWidth(1);
}

void sig::GFX::DrawText(const string& text, const Vector2& pos, Texture2D* bmpFont,
                        float scale, float spacing)
{
	if (bmpFont == nullptr) {
		return;
	}

	float tw = float(bmpFont->GetWidth() / 16);
	float th = float(bmpFont->GetHeight() / 16);
	float u2 = 1.0f / 16;
	float v2 = 1.0f / 16;

	bmpFont->Bind();
	glColor4f(fill.r, fill.g, fill.b, fill.a);

	glPushMatrix();
	glLoadIdentity();
	
	glTranslatef(pos.X(), pos.Y(), 0);
	glScalef(scale, scale, 1);

	for (unsigned int i = 0; i < text.size(); i++) {
		int c = int(text[i]);

		float u1 = (c % 16) * u2;
		float v1 = (c / 16) * v2;

		float m = spacing;

		DrawQuadFIX(Rect(u1, v1, u2, v2), Rect(i*(tw+m), 0, tw, th));
	}

	glPopMatrix();

	Texture2D::Unbind();
}

sig::math::Vector2 sig::GFX::GetTextMetrics(const string& text, Texture2D* bmpFont, float scale, float spacing)
{
	if (bmpFont == nullptr) {
		return Vector2::Zero;
	}

	float tw = float(bmpFont->GetWidth() / 16);
	float th = float(bmpFont->GetHeight() / 16);

	tw *= scale;
	th *= scale;

	return Vector2(text.size()*(tw+spacing), th);
}

void sig::GFX::DrawQuadFIX(const Rect& co, const Rect& r)
{
	glBegin(GL_QUADS);

	glTexCoord2f(co.x, co.y);
	glVertex2f(r.x, r.y);
	glTexCoord2f(co.x + co.width, co.y);
	glVertex2f(r.x+r.width, r.y);
	glTexCoord2f(co.x + co.width, co.y + co.height);
	glVertex2f(r.x+r.width, r.y+r.height);
	glTexCoord2f(co.x, co.y + co.height);
	glVertex2f(r.x, r.y+r.height);

	glEnd();
}

void sig::GFX::SetQuadUVs(float uvx1, float uvy1, float uvx2, float uvy2)
{
	float uvs[] = {
		uvx1, uvy1,
		uvx1, uvy2,
		uvx2, uvy1,
		uvx2, uvy2,
		uvx2, uvy1,
		uvx1, uvy2
	};
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, quadUV);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(float) * 12, uvs, GL_STATIC_DRAW_ARB);
}
