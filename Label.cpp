#include "Label.h"
#include "GUIGFX.h"
sig::Label::Label()
	:	Widget()
{
	m_text = "";
	m_fontScale = 1.0f;
	m_charSpacing = -10.0f;
	m_align = ALIGN_LEFT;
	m_textMargin = 0;
}

void sig::Label::Render()
{	
	float xa = m_textMargin;
	switch (m_align) {
		default:
		case ALIGN_LEFT: break;
		case ALIGN_CENTER:
			xa = GetBounds().width / 2 - Measure(m_text).X() / 2;
			break;
		case ALIGN_RIGHT:
			xa = (GetBounds().width - Measure(m_text).X()) - m_textMargin;
			break;
	}
	float ya = GetBounds().height / 2 - Measure(m_text).Y() / 2;

	GFX::SetFillColor(Color::BLACK);
	GFX::DrawText(m_text, Vector2(GetBounds().x + xa, GetBounds().y + ya + 1),
				  m_font, m_fontScale, m_charSpacing);

	GFX::SetFillColor(m_textColor);
	GFX::DrawText(m_text, Vector2(GetBounds().x + xa, GetBounds().y + ya),
				  m_font, m_fontScale, m_charSpacing);
}

sig::math::Vector2 sig::Label::Measure(const string& text)
{
	float cw = (float(m_font->GetWidth()) / 16.0f);
	float ch = (float(m_font->GetHeight()) / 16.0f);

	float xp = (cw + m_charSpacing) * m_fontScale;

	return Vector2((xp * text.size()), ch * m_fontScale);
}
