#include "Boolean.h"
#include "GUIGFX.h"

sig::Boolean::Boolean()
{
	m_selected = false;
	m_text = "Boolean";
}

void sig::Boolean::OnMouseDown(MouseEvent e)
{
	m_selected = !m_selected;
	if (m_selectedCallback) {
		m_selectedCallback();
	}
}

void sig::Boolean::Render()
{
	m_textMargin = 12;
	Label::Render();
	
	float cy = GetBounds().height/2 - 5;
	Rect box1 = Rect(2+GetBounds().x, GetBounds().y+cy, 10, 10);
	Rect box2 = box1.Inflated(-1, -1);
	
	GFX::SetFillColor(Color::BLACK);
	GFX::DrawRectangle(box1);
	GFX::SetFillColor(Color::WHITE);	
	GFX::DrawRectangle(box2);
	
	if (m_selected) {
		GFX::SetFillColor(Color::BLACK);
		GFX::DrawLine(Vector2(box1.x, box1.y), Vector2(box1.x+box1.width, box1.y+box1.height));
		GFX::DrawLine(Vector2(box1.x+box1.width, box1.y), Vector2(box1.x, box1.y+box1.height));
	}
}
