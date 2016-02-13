#include "Box.h"
#include "GUIGFX.h"

sig::Box::Box()
{
	l_order = 0;
	m_padding = 4;
	m_spacing = 4;
	m_orientation = ORIENTATION_VERTICAL;
}

void sig::Box::AddWidget(Widget* widget)
{
	if (widget == nullptr) {
		return;
	}

	l_order++;
	widget->l_order = l_order;
	widget->m_parent = this;

	m_widgets.push_back(widget);
}

void sig::Box::Render()
{
	sig::Widget::Render();

	GFX::SetFillColor(m_backColor.Brightness(0.2f));
	Rect r = Rect(GetBounds().x-1, GetBounds().y-1, GetBounds().width+2, GetBounds().height+3);
	GFX::DrawRectangle(r);

	GFX::SetFillColor(m_backColor);
	GFX::DrawRectangle(GetBounds());
}

void sig::Box::Update(float dt)
{
	int py = 0;
	int h = 0;
	for (auto it = m_widgets.begin(); it != m_widgets.end(); ++it) {
		Widget *w = (*it);
		if (m_orientation == ORIENTATION_VERTICAL) {
			h += w->m_bounds.height + m_spacing;
		} else {
			h += w->m_bounds.width + m_spacing;
		}
	}

	for (auto it = m_widgets.begin(); it != m_widgets.end(); ++it) {
		Widget *w = (*it);
		if (m_orientation == ORIENTATION_VERTICAL) {
			w->m_bounds.y = (m_padding + py) + (m_bounds.height/2-h/2);
			w->m_bounds.x = m_padding;
			w->m_bounds.width = m_bounds.width - m_padding * 2;

			py += w->m_bounds.height + m_spacing;
		} else {
			w->m_bounds.y = m_padding;
			w->m_bounds.x = (m_padding + py) + (m_bounds.width/2-h/2);
			w->m_bounds.height = m_bounds.height - m_padding * 2;

			py += w->m_bounds.width + m_spacing;
		}
	}
}
