#include "Button.h"
#include "GUIGFX.h"
#include <SDL2/SDL.h>

sig::Button::Button()
	:	Label()
{
	m_state = ButtonState::NORMAL;
	m_bounds.width = 120;
	m_align = ALIGN_CENTER;
}

void sig::Button::Render()
{
	GFX::SetFillColor(m_backColor.Brightness(0.2f));
	Rect r = Rect(GetBounds().x-1, GetBounds().y-1, GetBounds().width+2, GetBounds().height+3);
	GFX::DrawRectangle(r);
	
	switch (m_state) {
		case RELEASED:
		case NORMAL:
			GFX::SetFillColor(m_backColor);
			break;
		case HOVERED:
			GFX::SetFillColor(m_backColor.Brightness(1.2f));
			break;
		case CLICKED:
			GFX::SetFillColor(m_backColor.Brightness(0.7f));
			break;
	}
	GFX::DrawRectangle(GetBounds());
	
	Label::Render();
}

void sig::Button::OnMouseUp(MouseEvent e)
{
	if (m_state == CLICKED) {
		m_state = HOVERED;
		if (m_clickCallback) {
			m_clickCallback();
		}
	}
}

void sig::Button::OnMouseDown(MouseEvent e)
{
	if (m_state == HOVERED) {
		m_state = CLICKED;
	}
}

void sig::Button::OnMouseEnter()
{
	m_state = HOVERED;
}

void sig::Button::OnMouseLeave()
{
	m_state = NORMAL;
}
