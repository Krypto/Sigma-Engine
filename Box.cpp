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
	int current_widget_pos = 0;
	int size = 0;
	for (auto it = m_widgets.begin(); it != m_widgets.end(); ++it) {
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

	for (auto it = m_widgets.begin(); it != m_widgets.end(); ++it) {
		Widget *w = (*it);
		if (m_orientation == ORIENTATION_VERTICAL) {
			w->m_bounds.y = m_padding + current_widget_pos;
			w->m_bounds.x = m_padding;
			w->m_bounds.width = m_bounds.width - m_padding * 2;
			if (m_tab_left) {
				w->m_bounds.x += BOX_TAB_SIZE;
				w->m_bounds.width -= BOX_TAB_SIZE;
			}

			current_widget_pos += w->m_bounds.height + m_spacing;
		} else {
			w->m_bounds.y = m_padding;
			w->m_bounds.x = m_padding + current_widget_pos;
			w->m_bounds.height = m_bounds.height - m_padding * 2;

			current_widget_pos += w->m_bounds.width + m_spacing;
		}
	}
}

void sig::Box::Render()
{
	GFX::SetFillColor(m_backColor);
	GFX::DrawRectangle(GetBounds());
}
