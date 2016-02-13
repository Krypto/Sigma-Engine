#ifndef SIGMA_GUI_BOX
#define SIGMA_GUI_BOX

#include "Widget.h" // Base class: sig::Widget

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
		
		void Update(float dt);
		void Render();
		bool IsSensitive() { return false; }
		
		Box* SetPadding(int padding) {this->m_padding = padding; return this;}
		int GetPadding() const {return m_padding;}
		
		Box* SetSpacing(int spacing) {this->m_spacing = spacing; return this;}
		int GetSpacing() const {return m_spacing;}
		
		Box* SetOrientation(int orientation) {this->m_orientation = orientation; return this;}
		int GetOrientation() const {return m_orientation;}
				
		WType GetType() { return WType::BOX; }
	private:
		int m_padding, m_spacing;
		int l_order;
		int m_orientation;
		
		vector<Widget*> m_widgets;
	};

}

#endif // SIGMA_GUI_BOX
