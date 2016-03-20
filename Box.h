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
		
		int GetSpacing() const {return m_spacing;}
		void SetSpacing(int spacing) { this->m_spacing = spacing; }

		int GetOrientation() const { return m_orientation; }
		void SetOrientation(int orientation) { this->m_orientation = orientation; }

		bool IsTabbingWidgets() const { return m_tab_left; }
		void SetTabWidgets(bool tl) { m_tab_left = tl; }

		bool IsFitting() const { return m_fit; }
		void SetFitWidgets(bool fi) { m_fit = fi; }

		bool IsDrawingBackground() const { return m_draw_background; }
		void SetDrawBackground(bool db) { m_draw_background = db; }

		void Update(float dt);
		void Render();

		WIDGET_TYPE(WType::BOX)
	private:
		int m_padding, m_spacing;
		int l_order;
		int m_orientation;
		bool m_tab_left, m_fit, m_draw_background;
		
		WidgetList m_widgets;
	};

}

#endif // SIGMA_GUI_BOX
