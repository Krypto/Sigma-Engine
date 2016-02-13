#ifndef SIGMA_GUI_NUMBER_ENTRY
#define SIGMA_GUI_NUMBER_ENTRY

#include "Entry.h"
#include <functional>

namespace sig
{
	
	class NumberEntry : public Entry
	{
	public:
		NumberEntry();
		
		void OnKeyPress(int e);		
		void OnBlur(MouseEvent e);
		
		NumberEntry* SetValue(const float& value);
		float GetValue() const {return m_value;}
		
		void SetCallback(function<void(float)> callback) { m_changeCallback = callback; }
		
		WType GetType() { return WType::NUMBER; }
		
		void Update(float dt) { Entry::Update(dt); }
	private:
		float m_value;
		
		void SendToText();
		void SendToValue();
		
		function<void(float)> m_changeCallback;
	};

}

#endif // SIGMA_GUI_NUMBER_ENTRY
