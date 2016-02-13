#ifndef SIGMA_GUI_BUTTON
#define SIGMA_GUI_BUTTON

#include "Label.h" // Base class: sig::Label
#include <functional>

namespace sig
{

	class Button : public Label
	{
	public:
		enum ButtonState {
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
		
		WType GetType() { return WType::BUTTON; }
	private:
		ButtonState m_state;
		
		function<void()> m_clickCallback;
	};

}

#endif // SIGMA_GUI_BUTTON
