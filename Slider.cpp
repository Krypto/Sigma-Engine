#include "Slider.h"
#include "Input.h"
#include "GUIGFX.h"
#include "SIG_Utilities.h"

sig::Slider::Slider()
{
	m_min = 0;
	m_max = 10;
	m_value = 0;
	m_increment = 1;
	m_showValue = true;
	m_x = 0;

	m_bounds.height = 18;
	m_bounds.width = 120;
	m_align = ALIGN_CENTER;
	m_textMargin = 0;

	m_orientation = HORIZONTAL;

	m_prefix = "%";
}


void sig::Slider::OnMouseUp(MouseEvent e)
{
	if (e.button == Input::MOUSE_LEFT_BUTTON) {
		drag = false;
	}
}

void sig::Slider::OnMouseDown(MouseEvent e)
{
	if (e.button == Input::MOUSE_LEFT_BUTTON) {
		drag = true;
	}
}

void sig::Slider::OnMouseMove(MouseEvent e)
{
	if (drag) {
		float w = m_orientation == HORIZONTAL ? GetBounds().width : GetBounds().height;
		float px = m_orientation == HORIZONTAL ? e.position.X() : w-e.position.Y();

		m_x = abs(((m_min + m_value) / (m_min + m_max)) * (w + 0.0001f));

		if (px <= 0) {
			px = 0;
		} else if (px > w) {
			px = w;
		}

		float v = (px * (m_min+m_max)) / w;

		SetValue(round(v / m_increment) * m_increment);
	}
}

void sig::Slider::Render()
{
	GFX::SetFillColor(Color::BLACK);
	GFX::DrawRectangle(GetBounds().Inflated(1, 1));

	GFX::SetFillColor(m_backColor.Brightness(0.6f));
	GFX::DrawRectangle(GetBounds());

	Rect vr;
	if (m_orientation == VERTICAL) {
		float m_y = GetBounds().height - m_x;
		vr = Rect(GetBounds().x, GetBounds().y+m_y, GetBounds().width, m_x);
	} else {
		vr = Rect(GetBounds().x, GetBounds().y, m_x, GetBounds().height);
	}
	GFX::SetFillColor(m_backColor.Brightness(1.6f));
	GFX::DrawRectangle(vr);

	if (m_showValue) {
		if (drag) {
			m_text = ToString<float>(m_value) + m_prefix;
			Label::Render();
		}
	}
}

sig::Slider* sig::Slider::SetValue(float value)
{
	if (value != this->m_value) {
		this->m_value = value;
		if (m_changeCallback) {
			m_changeCallback();
		}
	}
	return this;
}
