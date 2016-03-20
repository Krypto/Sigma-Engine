#include "ListView.h"
#include "GUIGFX.h"
#include "Input.h"

sig::ListView::ListView()
{
	m_selected = -1;
}

sig::ListViewItem *sig::ListView::GetItem(int index)
{
	if (index < 0) { return nullptr; }
	return m_items[index];
}

void sig::ListView::SetSelected(int sel)
{
	if (sel != m_selected) {
		ListViewItem *prev = GetItem(m_selected);
		if (prev != nullptr) {
			prev->selected = false;
		}
		m_selected = sel;
		ListViewItem *selected = GetItem(m_selected);
		if (selected != nullptr) {
			selected->selected = true;
		}

		if (m_selectedCallback) {
			m_selectedCallback();
		}
	}
}

void sig::ListView::Render()
{
	Widget::Render();

	Color item_sel_color = m_backColor.Brightness(4.0f);
	item_sel_color.a = 0.4f;
	m_bounds.height = 0;

	float ch = (float(m_font->GetHeight()) / 16.0f);
	int index = 0;

	SIG_FOREACH(it, m_items)
	{
		ListViewItem *item = (*it);
		Rect item_rect = Rect(GetBounds().x,
							  GetBounds().y + index * item->item_height,
							  GetBounds().width, 0);

		GFX::SetFillColor(Color::WHITE);
		item->Render();

		item_rect.height = item->item_height;
		item->item_rect = item_rect;

		if (item->selected) {
			GFX::SetFillColor(item_sel_color);
			GFX::DrawRectangle(item_rect);
		}

		if (!item->ToString().empty()) {
			float ya = item_rect.height / 2 - ch / 2;
			GFX::SetFillColor(m_textColor);
			GFX::DrawText(item->ToString(),
						  Vector2(item_rect.x, item_rect.y + ya),
						  m_font, 1.0f, -10.0f);
		}

		m_bounds.height += item->item_height;
		index++;
	}
}

void sig::ListView::OnMouseDown(sig::MouseEvent e)
{
	int item_width = GetBounds().width;
	Rect item_rect = Rect(0, 0, item_width, 0);
	int index = 0;
	int ypos = 0;

	SIG_FOREACH(it, m_items)
	{
		ListViewItem *item = (*it);
		item_rect.height = item->item_height;
		item_rect.y = ypos;

		if (item_rect.HasPoint(e.position.X(), e.position.Y())) {
			SetSelected(index);
			break;
		}

		index++;
		ypos += item->item_height;
	}
}

void sig::Texture2DItem::Render()
{
	item_height = 32;
	Rect mr = item_rect;
	mr.width = 32;
	GFX::DrawRectangle(mr.Inflated(-1, -1), m_data);
}
