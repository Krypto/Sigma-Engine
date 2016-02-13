#ifndef SIGMA_GUI_BOOLEAN
#define SIGMA_GUI_BOOLEAN

#include "Label.h"

namespace sig
{

	class Boolean : public Label
	{
	public:
		Boolean();
		
		void OnMouseDown(MouseEvent e);
		void Render();
		
		Boolean* SetSelected(bool selected) {this->m_selected = selected; return this;}
		bool IsSelected() const {return m_selected;}
	private:
		bool m_selected;
	};

}

#endif // SIGMA_GUI_BOOLEAN
