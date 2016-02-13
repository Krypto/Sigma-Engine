#include "Widget.h"
#include "GUIGFX.h"
#include "SIG_Utilities.h"
#include "Box.h"
#include "Input.h"
#include "GUI.h"

sig::Widget::Widget()
{
	m_name = "widget";
	m_active = true;
	m_backColor = Color::FromColorAlpha(Color::DARK_GRAY, 0.8f);
	m_textColor = Color::WHITE;
	m_bounds = Rect(0, 0, 25, 25);
	m_parent = nullptr;
	m_font = GUI::DEFAULT_FONT;
	m_focused = false;
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
	Vector2 mp = Input::GetMousePosition();
	Rect b = GetBounds();

	MouseEvent e;

	if (b.HasPoint(mp.X(), mp.Y())) {
		e.hitWidget = this;
		e.position = mp - Vector2(b.x, b.y);

		if (!enter) {
			OnMouseEnter();
			enter = true;
		}

		if (Input::GetMouseButtonDown(SDL_BUTTON_LEFT)) {
			if (m_guiManager->m_focused != nullptr) {
				m_guiManager->m_focused->m_focused = false;
				m_guiManager->m_focused->OnBlur(e);
			}
			if (!focus) {
				OnFocus(e);
				focus = true;
			}
			
			m_focused = true;
			m_guiManager->m_focused = this;
			
			e.button = SDL_BUTTON_LEFT;
		} else if (Input::GetMouseButtonDown(SDL_BUTTON_RIGHT)) {
			e.button = SDL_BUTTON_RIGHT;
		} else if (Input::GetMouseButtonDown(SDL_BUTTON_MIDDLE)) {
			e.button = SDL_BUTTON_MIDDLE;
		}

		if (Input::GetMouseButtonUp(SDL_BUTTON_LEFT)) {
			e.button = SDL_BUTTON_LEFT;
		} else if (Input::GetMouseButtonUp(SDL_BUTTON_RIGHT)) {
			e.button = SDL_BUTTON_RIGHT;
		} else if (Input::GetMouseButtonUp(SDL_BUTTON_MIDDLE)) {
			e.button = SDL_BUTTON_MIDDLE;
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
		if (enter) {
			OnMouseLeave();
			enter = false;
			focus = false;
		}
		
//		if (Input::GetMouseButtonDown(SDL_BUTTON_LEFT)) {
//			if (m_guiManager->m_focused != nullptr) {
//				m_guiManager->m_focused->m_focused = false;
//				m_guiManager->m_focused->OnBlur(e);
//			}
//			m_guiManager->m_focused = nullptr;
//		}
		
		m_guiManager->handled = false;
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
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
//
//	GFX::SetFillColor(m_backColor);
//	GFX::DrawRectangle(m_bounds);
}
