#include "Entry.h"
#include "GUIGFX.h"
#include "Input.h"

#include <SDL2/SDL.h>
#include <algorithm>

sig::Entry::Entry()
	:	Label()
{
	accepted_keys = {
		Input::KEY_A, Input::KEY_B, Input::KEY_C, Input::KEY_D,
		Input::KEY_E, Input::KEY_F, Input::KEY_G, Input::KEY_H,
		Input::KEY_I, Input::KEY_J, Input::KEY_K, Input::KEY_L,
		Input::KEY_M, Input::KEY_N, Input::KEY_O, Input::KEY_P,
		Input::KEY_Q, Input::KEY_R, Input::KEY_S, Input::KEY_T,
		Input::KEY_U, Input::KEY_V, Input::KEY_W, Input::KEY_X,
		Input::KEY_Y, Input::KEY_Z, Input::KEY_0, Input::KEY_1,
		Input::KEY_2, Input::KEY_3, Input::KEY_4, Input::KEY_5,
		Input::KEY_6, Input::KEY_7, Input::KEY_8, Input::KEY_9,
		Input::KEY_SPACE, Input::KEY_MINUS, Input::KEY_COMMA,
		Input::KEY_PERIOD, Input::KEY_BACKSLASH, Input::KEY_SLASH,
		Input::KEY_EQUALS, Input::KEY_KP_0, Input::KEY_KP_1, 
		Input::KEY_KP_2, Input::KEY_KP_3, Input::KEY_KP_4, 
		Input::KEY_KP_5, Input::KEY_KP_6, Input::KEY_KP_7,
		Input::KEY_KP_8, Input::KEY_KP_9, Input::KEY_KP_PLUS,
		Input::KEY_KP_MINUS, Input::KEY_KP_PERIOD, Input::KEY_KP_COMMA,
		Input::KEY_KP_DIVIDE, Input::KEY_KP_MULTIPLY
	};
	
	m_caretx = 0;
	m_text = "";
	m_backColor = Color::WHITE;
	m_textColor = Color::BLACK;
	m_blink = false;
	m_masked = false;
	m_btime = 0;
	m_bounds.height = 18;
}

void sig::Entry::OnMouseDown(MouseEvent e)
{
	float xpos = 0;
	for (u32 i = 0; i <= m_text.size(); i++) {
		if (xpos > GetBounds().width) { break; }
		
		char c = m_text[i];
		string charS(1, c);
		Vector2 cs = Measure(charS);
		
		float w = cs.X();
		if (i >= m_text.size()) {
			w = GetBounds().width - xpos;
		}
		
		Rect charRect = Rect(xpos, 0, w, GetBounds().height);
		if (charRect.HasPoint(e.position.X(), e.position.Y())) {
			m_caretx = i;
			m_blink = true;
			m_btime = 0;
			break;
		}
		
		xpos += cs.X();
	}
}

void sig::Entry::OnKeyPress(int e)
{
	// Caret movement
	if (e == Input::KEY_LEFT) {
		if (m_caretx > 0) {
			m_caretx--;
		}
	} else if (e == Input::KEY_RIGHT) {
		if (m_caretx < m_text.size()) {
			m_caretx++;
		}
	} else
	
	// Erase text
	if (e == Input::KEY_BACKSPACE) {
		if (m_caretx > 0) {
			m_caretx--;
			m_text.erase(m_text.begin() + m_caretx);
		}
	} else if (e == Input::KEY_DELETE) {
		string sub = m_text.substr(m_caretx);
		if (!sub.empty()) {
			m_text.erase(m_text.begin() + m_caretx);
		}
	} else
	
	// Home and End
	if (e == Input::KEY_HOME) {
		m_caretx = 0;
	} else if (e == Input::KEY_END) {
		m_caretx = m_text.size();
	} else 
	
	// Actual typing
	if (std::find(accepted_keys.begin(), accepted_keys.end(), e) != accepted_keys.end()) {
		string keyStr = string(SDL_GetScancodeName((SDL_Scancode)e));
		if (keyStr.find("Keypad ", 0) != string::npos) {
			keyStr.replace(0, 7, "");
		}
		if (keyStr == "Space") {
			keyStr = " ";
		}
		
		if (!Input::GetModifier(Input::MOD_SHIFT)) {
			std::transform(keyStr.begin(), keyStr.end(), keyStr.begin(), ::tolower);
		}
		
		m_text.insert(m_caretx, 1, keyStr.at(0));
		m_caretx++;
	}
	
	if (m_changeCallback) {
		m_changeCallback();
	}

	m_blink = true;
	m_btime = 0;
}

void sig::Entry::SetText(const string &text)
{
	Label::SetText(text);
	if (m_caretx > m_text.size()) {
		m_caretx = m_text.size();
	}
}

void sig::Entry::Render()
{
	sig::Widget::Render();
	
	GFX::SetFillColor(Color::BLACK);
	Rect r = GetBounds().Inflated(-1, -1);
	GFX::DrawRectangle(GetBounds());
	
	GFX::SetFillColor(m_backColor);
	GFX::DrawRectangle(r);
	GFX::SetFillColor(m_textColor);
	
	if (!m_text.empty()) {
		float xpos = 0;
		for (u32 i = 0; i < m_text.size(); i++) {
			char c = m_text[i];
			string charS(1, c);
			
			Vector2 cs = Measure(charS);
			
			if (GetBounds().x + xpos - cs.X() > GetBounds().x + GetBounds().width) { continue; }
			
			if (m_masked) {
				charS = "*";
			}

			GFX::DrawText(charS, Vector2(GetBounds().x+xpos, GetBounds().y),
							m_font, m_fontScale, m_charSpacing);
			
			xpos += cs.X();
		}
	}
	
	if (m_focused && m_blink) {
		float cw = (float(m_font->GetWidth()) / 16.0f);
		
		float sz = (cw + m_charSpacing) * m_fontScale;
		float tsz = sz * m_caretx;
		float m = tsz - sz/2;
		float cursorPos = GetBounds().x + m;
		
		if (cursorPos-sz < GetBounds().x+GetBounds().width) {
			GFX::DrawText("|", Vector2(GetBounds().x + m, GetBounds().y),
							m_font, m_fontScale, m_charSpacing);
		}
	}
}

void sig::Entry::Update(float dt)
{
	sig::Widget::Update(dt);
	
	if (m_focused) {
		m_btime += dt;
		if (m_btime >= 0.5f) {
			m_btime = 0;
			m_blink = !m_blink;
		}
	}
}
