#include "Button.h"
#include "GUIGFX.h"
#include <SDL2/SDL.h>

sig::Button::Button()
	:	Label()
{
	m_state = ButtonState::NORMAL;
	m_bounds.width = 90;
	m_bounds.height = 12;
	m_backColor = Color::DARK_BLUE;
}

void sig::Button::Render()
{
	Rect r = GetBounds().Inflated(-1, -1);
	Color fcol = m_backColor;

	switch (m_state) {
		case ButtonState::RELEASED:
		case ButtonState::NORMAL:
			fcol = m_backColor;
			break;
		case ButtonState::HOVERED:
			fcol = m_backColor.Brightness(1.2f);
			break;
		case ButtonState::CLICKED:
			fcol = m_backColor.Brightness(0.8f);
			break;
	}
	GFX::SetFillColor(fcol);
	GFX::DrawRectangle(r);
	
	Label::Render();
}

void sig::Button::OnMouseUp(MouseEvent e)
{
	if (m_state == ButtonState::CLICKED) {
		m_state = ButtonState::HOVERED;
		if (m_clickCallback) {
			m_clickCallback();
		}
	}
}

void sig::Button::OnMouseDown(MouseEvent e)
{
	if (m_state == ButtonState::HOVERED) {
		m_state = ButtonState::CLICKED;
	}
}

void sig::Button::OnMouseEnter()
{
	m_state = ButtonState::HOVERED;
}

void sig::Button::OnMouseLeave()
{
	m_state = ButtonState::NORMAL;
}
