#ifndef SIGMA_GUI_BOOLEAN
#define SIGMA_GUI_BOOLEAN

#include "Label.h"
#include <functional>

namespace sig
{

	class Boolean : public Label
	{
	public:
		Boolean();
		
		void OnMouseDown(MouseEvent e);
		void Render();
		
		void SetSelected(bool selected) { this->m_selected = selected; }
		bool IsSelected() const { return m_selected; }

		void SetCallback(std::function<void()> cb) { m_selectedCallback = cb; }

		WIDGET_TYPE(WType::BOOLEAN)
	private:
		bool m_selected;
		std::function<void()> m_selectedCallback;
	};

}

#endif // SIGMA_GUI_BOOLEAN
