#ifndef SIGMA_GUI_BOX
#define SIGMA_GUI_BOX

#include "Widget.h" // Base class: sig::Widget

#define BOX_TAB_SIZE 16

namespace sig
{
	class Box : public Widget
	{
	public:
		enum {
			ORIENTATION_VERTICAL = 2,
			ORIENTATION_HORIZONTAL = 4
		};
		
		Box();

		void AddWidget(Widget* widget);
		bool IsSensitive() { return false; }
		
		void SetPadding(int padding) { this->m_padding = padding; }
		int GetPadding() const {return m_padding;}
		
		void SetSpacing(int spacing) { this->m_spacing = spacing; }
		int GetSpacing() const {return m_spacing;}
		
		void SetOrientation(int orientation) { this->m_orientation = orientation; }
		int GetOrientation() const { return m_orientation; }

		void SetTabWidgets(bool tl) { m_tab_left = tl; }
		bool IsTabbingWidgets() const { return m_tab_left; }

		void Update(float dt);
		void Render();

		WIDGET_TYPE(WType::BOX)
	private:
		int m_padding, m_spacing;
		int l_order;
		int m_orientation;
		bool m_tab_left;
		
		WidgetList m_widgets;
	};

}

#endif // SIGMA_GUI_BOX
