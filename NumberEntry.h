#ifndef SIGMA_GUI_NUMBER_ENTRY
#define SIGMA_GUI_NUMBER_ENTRY

#include "Entry.h"

namespace sig
{
	
	class NumberEntry : public Entry
	{
	public:
		NumberEntry();
		
		void OnKeyPress(int e);		
		void OnBlur(MouseEvent e);
		
		float GetValue() const { return m_value; }
		void SetValue(float value);

		void Update(float dt) { Entry::Update(dt); }

		void SetValueCallback(function<void()> cb) { m_valueCallback = cb; }

		WIDGET_TYPE(WType::NUMBER)
	private:
		float m_value;
		
		void SendToText();
		void SendToValue();

		function<void()> m_valueCallback;
	};

}

#endif // SIGMA_GUI_NUMBER_ENTRY
