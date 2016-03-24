#include "Widget.h"
#include "GUIGFX.h"
#include "SIG_Utilities.h"
#include "Box.h"
#include "Input.h"
#include "GUI.h"

sig::Widget::Widget()
{
	m_active = true;
	m_backColor = Color::FromColorAlpha(Color::DARK_BLUE, 0.5f);
	m_textColor = Color::WHITE;
	m_bounds = Rect(0, 0, 25, 25);
	m_parent = nullptr;
	m_font = GUI::DEFAULT_FONT;
	m_focused = false;
	m_dockDir = DockDirection::DOCK_NONE;
}

sig::Widget::~Widget()
{
}

sig::Rect sig::Widget::GetBounds()
{
	Rect parentRect;
	if (m_parent != nullptr) {
		parentRect = m_parent->GetBounds();
	}

	return Rect(m_bounds.x + parentRect.x,
	            m_bounds.y + parentRect.y,
				m_bounds.width, m_bounds.height);
}

void sig::Widget::Update(float dt)
{
	Rect b = GetBounds();

	if (m_dockDir != DockDirection::DOCK_NONE) {
		Rect parentRect;
		if (m_parent != nullptr) {
			parentRect = m_parent->GetBounds();
		} else {
			parentRect = m_guiManager->GetWindowDimensions();
		}
		switch (m_dockDir) {
			case DockDirection::DOCK_LEFT: {
				b.y = 0;
				b.x = 0;
				b.height = parentRect.height;
			} break;
			case DockDirection::DOCK_RIGHT: {
				b.y = 0;
				b.x = parentRect.width - b.width;
				b.height = parentRect.height;
			} break;
			case DockDirection::DOCK_TOP: {
				b.y = 0;
				b.x = 0;
				b.width = parentRect.width;
			} break;
			case DockDirection::DOCK_BOTTOM: {
				b.y = parentRect.height - b.height;
				b.x = 0;
				b.width = parentRect.width;
			} break;
			case DockDirection::DOCK_FILL: {
				b.y = 0;
				b.x = 0;
				b.height = parentRect.height;
				b.width = parentRect.width;
			} break;
			case DockDirection::DOCK_NONE: break;
		}
		SetBounds(b);
	}

	Vector2 mp = Input::GetMousePosition();

	MouseEvent e;
	e.position = mp - Vector2(b.x, b.y);
	e.button = Input::GetMouseButton();

	if (b.HasPoint(mp.X(), mp.Y())) {
		e.hitWidget = this;

		if (!enter) {
			OnMouseEnter();
			enter = true;
		}

		if (Input::GetMouseButtonDown(SDL_BUTTON_LEFT)) {
			if (m_guiManager->m_focused != nullptr) {
				m_guiManager->m_focused->m_focused = false;
				m_guiManager->m_focused->OnBlur(e);
			}
			if (!m_focused) {
				OnFocus(e);
				m_focused = true;
				m_guiManager->m_focused = this;
			}
		}

		if (!m_guiManager->handled) {
			if (Input::GetMouseButtonDown(SDL_BUTTON_LEFT)		||
			    Input::GetMouseButtonDown(SDL_BUTTON_RIGHT)		||
				Input::GetMouseButtonDown(SDL_BUTTON_MIDDLE)) {
				if (!click) {
					OnMouseDown(e);
					click = true;
				}
				m_guiManager->handled = true;
			}
		}

		if (Input::GetMouseButtonUp(SDL_BUTTON_LEFT)	||
			Input::GetMouseButtonUp(SDL_BUTTON_RIGHT)	||
			Input::GetMouseButtonUp(SDL_BUTTON_MIDDLE)) {
			if (click) {
				OnMouseUp(e);
				click = false;
			}
			m_guiManager->handled = false;
		}
	} else {
		if (!click) {
			if (enter) {
				OnMouseLeave();
				enter = false;
				focus = false;
			}
			m_guiManager->handled = false;
		} else {
			if (Input::GetMouseButtonUp(SDL_BUTTON_LEFT)	||
				Input::GetMouseButtonUp(SDL_BUTTON_RIGHT)	||
				Input::GetMouseButtonUp(SDL_BUTTON_MIDDLE)) {
				if (m_guiManager->m_focused != nullptr) {
					m_guiManager->m_focused->m_focused = false;
					m_guiManager->m_focused->OnBlur(e);
				}
				click = false;
				OnMouseUp(e);
				m_guiManager->handled = false;
			}
		}
	}
	
	if (m_focused) {
		for (int i = 0; i < MAX_KEYS; i++) {
			if (Input::GetKeyDown(i)) {
				OnKeyPress(i);
				break;
			}
		}
		OnMouseMove(e);
	}
}

void sig::Widget::Render()
{
	GFX::SetFillColor(m_backColor);
	GFX::DrawRectangle(GetBounds());
}
