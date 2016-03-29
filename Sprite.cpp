#include "Sprite.h"
#include "SpriteBatch.h"
#include "Node.h"

void sig::Sprite::Render(SpriteBatch* batch)
{
	if (m_texture != nullptr) {
		m_owner->m_size = Vector2(m_texture->GetWidth() * m_uvRectangle.width,
								  m_texture->GetHeight() * m_uvRectangle.height);
	}
	if (batch == nullptr) { return; }
	if (batch->IsDrawing()) {
		if (m_owner != nullptr) {
			batch->Draw(m_owner->GetTransformMatrix(), this);
		}
	}
}

sig::Sprite *sig::Sprite::GetInstance(Node *owner)
{
	Sprite *inst = new Sprite(m_texture, m_shader);
	inst->m_uvRectangle = Rect(m_uvRectangle.x, m_uvRectangle.y,
							   m_uvRectangle.width, m_uvRectangle.height);
	inst->m_color = Color(m_color.r, m_color.g, m_color.b, m_color.a);
	inst->m_owner = owner;

	return inst;
}

int sig::Sprite::GetDrawOrder() const
{
    return m_drawOrder;
}

void sig::Sprite::SetDrawOrder(int drawOrder)
{
    m_drawOrder = drawOrder;
}


sig::Sprite::Sprite()
    :	Sprite(nullptr, GPUShaders::SIG_GPU_SPRITEDEFAULT)
{}

sig::Sprite::Sprite(Texture2D* tex)
    :	Sprite(tex, GPUShaders::SIG_GPU_SPRITEDEFAULT)
{}

sig::Sprite::Sprite(Texture2D *tex, Shader *shad)
	:	Component()
{
	m_texture = tex;
	m_shader = shad;
	m_uvRectangle = Rect(0, 0, 1, 1);
	m_color = Color::WHITE;
	m_drawOrder = 0;
}

