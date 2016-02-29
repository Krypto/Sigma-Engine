#include "SpriteBatch.h"
#include <algorithm>

int sig::SpriteBatch::drawOrder = 0;

sig::SpriteBatch::SpriteBatch()
{
	m_vao = 0;
	m_vbo = 0;
	m_drawing = false;
}

sig::SpriteBatch::~SpriteBatch()
{
	if (m_vao) { glDeleteVertexArrays(1, &m_vao); }
	if (m_vbo) { glDeleteBuffers(1, &m_vbo); }
}

void sig::SpriteBatch::Draw(const math::Matrix4 &transform, Shader* shader, float w, float h, const Rect& uv, Texture2D *texture, int z, const Color& col)
{
	Glyph *newGlyph = new Glyph();
	newGlyph->shader = shader;
	newGlyph->z = z;
	newGlyph->texture = texture;
	newGlyph->transform = transform;

	// Store positions
	newGlyph->topLeft.position = Vector2(0, 0);
	newGlyph->topRight.position = Vector2(w, 0);
	newGlyph->bottomLeft.position = Vector2(0, h);
	newGlyph->bottomRight.position = Vector2(w, h);

	// Store UVs
	newGlyph->topLeft.uv = Vector2(uv.x, uv.y);
	newGlyph->topRight.uv = Vector2(uv.x + uv.width, uv.y);
	newGlyph->bottomLeft.uv = Vector2(uv.x, uv.y + uv.height);
	newGlyph->bottomRight.uv = Vector2(uv.x + uv.width, uv.y + uv.height);

	// Store Colors
	newGlyph->topLeft.color = col;
	newGlyph->topRight.color = col;
	newGlyph->bottomLeft.color = col;
	newGlyph->bottomRight.color = col;

	Draw(newGlyph);
}

void sig::SpriteBatch::Draw(Glyph* glyph)
{
	m_glyphs.push_back(glyph);
}

void sig::SpriteBatch::Draw(const math::Matrix4& transform, Sprite* sprite)
{
	float w 		= sprite->GetTexture() != nullptr ? sprite->GetTexture()->GetWidth() : 10;
	float h 		= sprite->GetTexture() != nullptr ? sprite->GetTexture()->GetHeight() : 10;
	
	w *= sprite->GetUVRectangle().width;
	h *= sprite->GetUVRectangle().height;
	
	Draw(	transform, sprite->GetShader(), w, h,
			sprite->GetUVRectangle(), sprite->GetTexture(),
			sprite->GetDrawOrder(), sprite->GetColor());
}

void sig::SpriteBatch::Begin(GlyphSortingMode sortingMode)
{
	drawOrder = 0;
	m_sortingMode = sortingMode;
	m_renderBatches.clear();
	
	for (auto it = m_glyphs.begin(); it != m_glyphs.end(); ++it) {
		SIG_FREE(*it);
	}
	
	m_glyphs.clear();
	
	m_drawing = true;
}

void sig::SpriteBatch::End()
{
	SortGlyphs();
	CreateRenderBatches();
	m_drawing = false;
}

void sig::SpriteBatch::Initialize()
{
	CreateVAO();
}

void sig::SpriteBatch::CreateVAO()
{
	if (m_vao == 0) {
		glGenVertexArrays(1, &m_vao);
	}
	glBindVertexArray(m_vao);
	if (m_vbo == 0) {
		glGenBuffers(1, &m_vbo);
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glVertexAttribPointer(2, 4, GL_FLOAT, true, sizeof(Vertex), (void*)offsetof(Vertex, color));

	glBindVertexArray(0);
}

void sig::SpriteBatch::SortGlyphs()
{
	switch (m_sortingMode) {
		case GlyphSortingMode::BACK_TO_FRONT:
			std::stable_sort(m_glyphs.begin(), m_glyphs.end(), CompareBACK_TO_FRONT);
			break;
		case GlyphSortingMode::FRONT_TO_BACK:
			std::stable_sort(m_glyphs.begin(), m_glyphs.end(), CompareFRONT_TO_BACK);
			break;
		case GlyphSortingMode::TEXTURE:
			std::stable_sort(m_glyphs.begin(), m_glyphs.end(), CompareTEXTURE);
			break;
		default:
			break;
	}
}

void sig::SpriteBatch::CreateRenderBatches()
{
	vector<Vertex> verts;
	verts.resize(m_glyphs.size() * 6);

	if (m_glyphs.empty()) {
		return;
	}

	int offset = 0;
	int cv = 0;
	m_renderBatches.emplace_back(m_glyphs[0]->shader, offset, 6, m_glyphs[0]->texture, m_glyphs[0]->transform);

	verts[cv++] = m_glyphs[0]->topLeft;
	verts[cv++] = m_glyphs[0]->bottomLeft;
	verts[cv++] = m_glyphs[0]->bottomRight;
	verts[cv++] = m_glyphs[0]->bottomRight;
	verts[cv++] = m_glyphs[0]->topRight;
	verts[cv++] = m_glyphs[0]->topLeft;

	offset += 6;

	for (u32 cg = 1; cg < m_glyphs.size(); cg++) {
		m_renderBatches.emplace_back(m_glyphs[cg]->shader, offset, 6, m_glyphs[cg]->texture, m_glyphs[cg]->transform);

		verts[cv++] = m_glyphs[cg]->topLeft;
		verts[cv++] = m_glyphs[cg]->bottomLeft;
		verts[cv++] = m_glyphs[cg]->bottomRight;
		verts[cv++] = m_glyphs[cg]->bottomRight;
		verts[cv++] = m_glyphs[cg]->topRight;
		verts[cv++] = m_glyphs[cg]->topLeft;
		offset += 6;
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, verts.size() * sizeof(Vertex), verts.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
}

void sig::SpriteBatch::Render()
{
	glBindVertexArray(m_vao);
	
	for (auto it = m_renderBatches.begin(); it != m_renderBatches.end(); ++it) {
		RenderBatch b = *it;
		
		glPushMatrix();
		glMultMatrixf(&b.transform(0, 0));
		
		if (b.texture != nullptr) {
			b.texture->Bind(0);
		}
		
		if (b.shader != nullptr) {
			if (b.shader->IsValid()) {
				b.shader->Bind();
				b.shader->SetInt("tex0", 0);
			}
		}
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDrawArrays(GL_TRIANGLES, b.offset, b.numVertices);
		glDisable(GL_BLEND);
		glPopMatrix();
		
		Texture2D::Unbind();
		Shader::Unbind();
	}
	
	glBindVertexArray(0);
	glEnable(GL_BLEND);
}

bool sig::SpriteBatch::CompareFRONT_TO_BACK(Glyph* a, Glyph* b)
{
	return a->z < b->z;
}

bool sig::SpriteBatch::CompareBACK_TO_FRONT(Glyph* a, Glyph* b)
{
	return a->z > b->z;
}

bool sig::SpriteBatch::CompareTEXTURE(Glyph* a, Glyph* b)
{
	return a->texture < b->texture;
}

void sig::SpriteBatch::Draw(float x, float y, Sprite* sprite)
{
	Draw(x, y, 1, 1, 0, 0, 0, sprite);
}

void sig::SpriteBatch::Draw(float x, float y, float scalex, float scaley, Sprite* sprite)
{
	Draw(x, y, scalex, scaley, 0, 0, 0, sprite);
}

void sig::SpriteBatch::Draw(float x, float y, float scalex, float scaley, float orix, float oriy, float rot, Sprite* sprite)
{
	float w 		= sprite->GetTexture() != nullptr ? sprite->GetTexture()->GetWidth() : 1;
	float h 		= sprite->GetTexture() != nullptr ? sprite->GetTexture()->GetHeight() : 1;
	
	Matrix4 pos = Matrix4::MakeTranslation(Vector3(x, y, 0));
	Matrix4 rotm = Matrix4::MakeRotation(rot, Vector3(0, 0, 1));
	Matrix4 ori = Matrix4::MakeTranslation(Vector3(-orix * w, -oriy * h, 0));
	Matrix4 siz = Matrix4::MakeScale(Vector3(scalex, scaley, 1));

	Matrix4 t = pos * siz * rotm * ori;
	
	Draw(t, sprite);
}

void sig::SpriteBatch::Draw(float x, float y, float orix, float oriy, float rot, Sprite* sprite)
{
	Draw(x, y, 1, 1, orix, oriy, rot, sprite);
}

void sig::SpriteBatch::Draw(float x, float y, float rot, Sprite* sprite)
{
	Draw(x, y, 1, 1, 0, 0, rot, sprite);
}

void sig::SpriteBatch::Draw(float x, float y, float scalex, float scaley, float orix, float oriy, float rot, const Rect& uv, Sprite* sprite)
{
	float w 		= sprite->GetTexture() != nullptr ? sprite->GetTexture()->GetWidth() : 1;
	float h 		= sprite->GetTexture() != nullptr ? sprite->GetTexture()->GetHeight() : 1;
	w *= uv.width;
	h *= uv.height;

	Matrix4 pos = Matrix4::MakeTranslation(Vector3(x, y, 0));
	Matrix4 rotm = Matrix4::MakeRotation(rot, Vector3(0, 0, 1));
	Matrix4 ori = Matrix4::MakeTranslation(Vector3(-orix * w, -oriy * h, 0));
	Matrix4 siz = Matrix4::MakeScale(Vector3(scalex, scaley, 1));

	Matrix4 t = pos * siz * rotm * ori;
	
	Draw(t, sprite->GetShader(), w, h, uv, sprite->GetTexture(), sprite->GetDrawOrder(), sprite->GetColor());
}

void sig::SpriteBatch::Draw(int order, float x, float y, float scalex, float scaley, float orix, float oriy, float rot, const sig::Rect &uv, sig::Sprite *sprite)
{
	float w 		= sprite->GetTexture() != nullptr ? sprite->GetTexture()->GetWidth() : 1;
	float h 		= sprite->GetTexture() != nullptr ? sprite->GetTexture()->GetHeight() : 1;
	w *= uv.width;
	h *= uv.height;

	Matrix4 pos = Matrix4::MakeTranslation(Vector3(x, y, 0));
	Matrix4 rotm = Matrix4::MakeRotation(rot, Vector3(0, 0, 1));
	Matrix4 ori = Matrix4::MakeTranslation(Vector3(-orix * w, -oriy * h, 0));
	Matrix4 siz = Matrix4::MakeScale(Vector3(scalex, scaley, 1));

	Matrix4 t = pos * siz * rotm * ori;

	Draw(t, sprite->GetShader(), w, h, uv, sprite->GetTexture(), order, sprite->GetColor());
}
