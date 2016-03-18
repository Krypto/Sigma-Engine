#include "Image.h"
#include "GUIGFX.h"

sig::Image::Image()
{
	m_image = nullptr;
}

void sig::Image::Render()
{
	Widget::Render();
	Rect ir = GetBounds().Inflated(-2, -2);

	GFX::SetFillColor(Color::WHITE);
	GFX::DrawRectangle(ir, m_image);
}
