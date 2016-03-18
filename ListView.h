#ifndef SIGMA_GUI_LIST_VIEW
#define SIGMA_GUI_LIST_VIEW

#include "Widget.h"

#include <vector>
#include <string>
#include <functional>
using namespace std;

namespace sig
{
	class ListViewItem
	{
	public:
		ListViewItem()
			:	item_height(12), selected(false)
		{}

		virtual void Render() {}
		virtual string ToString() { return "ListViewItem"; }

		int item_height;
		bool selected;
		Rect item_rect;
	};

	class StringItem : public ListViewItem
	{
	public:
		StringItem()
			:	m_data("")
		{}
		StringItem(const string& txt)
			:	m_data(txt)
		{}

		virtual string ToString() override { return m_data; }

		string& GetData() { return m_data; }
		void SetData(const string& str) { m_data = str; }
	private:
		string m_data;
	};

	class Texture2DItem : public ListViewItem
	{
	public:
		Texture2DItem()
			:	m_data(nullptr)
		{}
		Texture2DItem(Texture2D* tex)
			:	m_data(tex)
		{}

		virtual void Render();
		virtual string ToString() override { return ""; }

		Texture2D* GetData() { return m_data; }
		void SetData(Texture2D* data) { m_data = data; }
	private:
		Texture2D *m_data;
	};

	typedef vector<ListViewItem*> ItemList;

	class ListView : public Widget
	{
	public:
		ListView();

		ListViewItem* GetItem(int index);
		ItemList& GetItems() { return m_items; }
		void SetItems(const ItemList& items) { m_items = items; }

		int GetSelected() const { return m_selected; }
		void SetSelected(int sel);

		void SetCallback(function<void()> cb) { m_selectedCallback = cb; }

		void Render();
		void OnMouseDown(MouseEvent e);

		WIDGET_TYPE(WType::LIST)
	private:
		ItemList m_items;
		int m_selected;
		function<void()> m_selectedCallback;
	};

}

#endif // SIGMA_GUI_LIST_VIEW
