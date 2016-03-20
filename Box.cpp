#include "Box.h"
#include "GUIGFX.h"

#include <algorithm>

sig::Box::Box()
{
	l_order = 0;
	m_padding = 4;
	m_spacing = 2;
	m_orientation = ORIENTATION_VERTICAL;
	m_backColor = Color::FromColorAlpha(Color::DARK_BLUE, 0.4f);
	m_tab_left = false;
	m_fit = false;
	m_draw_background = true;
}

void sig::Box::AddWidget(Widget* widget)
{
	if (widget == nullptr || widget == this) {
		return;
	}

	auto pos = std::find(m_widgets.begin(), m_widgets.end(), widget);
	if (pos == m_widgets.end()) {
		l_order++;
		widget->l_order = l_order;
		widget->m_parent = this;
		m_widgets.push_back(widget);
	}
}

void sig::Box::Update(float dt)
{
	Widget::Update(dt);

	int current_widget_pos = 0;

	if (!m_fit) {
		int size = 0;
		SIG_FOREACH(it, m_widgets)
		{
			Widget *w = (*it);
			if (m_orientation == ORIENTATION_VERTICAL) {
				size += w->m_bounds.height + m_spacing;
			} else {
				size += w->m_bounds.width + m_spacing;
			}
		}

		size += m_padding * 2;
		if (m_orientation == ORIENTATION_VERTICAL) {
			m_bounds.height = size;
		} else {
			m_bounds.width = size;
		}
	}

	int padding_size = (m_widgets.size() * m_spacing);

	SIG_FOREACH(it, m_widgets)
	{
		Widget *w = (*it);
		if (m_orientation == ORIENTATION_VERTICAL) {
			if (m_fit) {
				int wsize = m_bounds.height / m_widgets.size();
				w->m_bounds.height = wsize + padding_size;
			}

			w->m_bounds.y = m_padding + current_widget_pos;
			w->m_bounds.x = m_padding;
			w->m_bounds.width = m_bounds.width - m_padding * 2;
			if (m_tab_left) {
				w->m_bounds.x += BOX_TAB_SIZE;
				w->m_bounds.width -= BOX_TAB_SIZE;
			}

			current_widget_pos += w->m_bounds.height + m_spacing;
		} else {
			if (m_fit) {
				int wsize = m_bounds.width / m_widgets.size();
				w->m_bounds.width = wsize - (padding_size + m_padding/2);
			}

			w->m_bounds.y = m_padding;
			w->m_bounds.x = m_padding + current_widget_pos;
			w->m_bounds.height = m_bounds.height - m_padding * 2;

			current_widget_pos += w->m_bounds.width + m_spacing;
		}
	}
}

void sig::Box::Render()
{
	if (m_draw_background) {
		GFX::SetFillColor(m_backColor);
		GFX::DrawRectangle(GetBounds());
	}
}
