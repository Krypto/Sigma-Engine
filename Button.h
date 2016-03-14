#ifndef SIGMA_GUI_BUTTON
#define SIGMA_GUI_BUTTON

#include "Label.h"
#include <functional>

namespace sig
{

	class Button : public Label
	{
	public:
		enum class ButtonState {
			RELEASED,
			CLICKED,
			HOVERED,
			NORMAL,
		};
		
		Button();
		
		void Render();
		
		void OnMouseUp(MouseEvent e);
		void OnMouseDown(MouseEvent e);
		void OnMouseEnter();
		void OnMouseLeave();
		
		void SetCallback(function<void()> callback) { m_clickCallback = callback; }
		
		WIDGET_TYPE(WType::BUTTON)
	private:
		ButtonState m_state;
		
		function<void()> m_clickCallback;
	};

}

#endif // SIGMA_GUI_BUTTON
