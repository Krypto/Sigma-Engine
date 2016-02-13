#include "Label.h"
#include "GUIGFX.h"
sig::Label::Label()
	:	Widget()
{
	m_text = "";
	m_fontScale = 0.6f;
	m_charSpacing = -22.0f;
	m_align = ALIGN_LEFT;
	m_textMargin = 0;
}

void sig::Label::Render()
{
	sig::Widget::Render();

	GFX::SetFillColor(m_textColor);
	
	float xa = m_textMargin;
	switch (m_align) {
		default:
		case ALIGN_LEFT: break;
		case ALIGN_CENTER:
			xa = GetBounds().width/2 - Measure(m_text).X()/2;
			break;
		case ALIGN_RIGHT:
			xa = (GetBounds().width - Measure(m_text).X()) + m_textMargin;
			break;
	}
	
	GFX::DrawText(m_text, Vector2(GetBounds().x+xa, GetBounds().y), m_font, m_fontScale, m_charSpacing);
}

sig::math::Vector2 sig::Label::Measure(const string& text)
{
	float cw = (float(m_font->GetWidth()) / 16.0f);
	float ch = (float(m_font->GetHeight()) / 16.0f);

	float xp = (cw+m_charSpacing) * m_fontScale;

	return Vector2((xp * text.size()), (ch+m_charSpacing) * m_fontScale);
}
