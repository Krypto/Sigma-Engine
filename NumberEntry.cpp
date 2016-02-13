#include "NumberEntry.h"
#include "SIG_Utilities.h"
#include "Input.h"

sig::NumberEntry::NumberEntry()
{
	accepted_keys = {
		Input::KEY_0, Input::KEY_1,	Input::KEY_2, Input::KEY_3,
		Input::KEY_4, Input::KEY_5,	Input::KEY_6, Input::KEY_7,
		Input::KEY_8, Input::KEY_9,	Input::KEY_MINUS, Input::KEY_COMMA,
		Input::KEY_PERIOD, Input::KEY_KP_0, Input::KEY_KP_1,
		Input::KEY_KP_2, Input::KEY_KP_3, Input::KEY_KP_4,
		Input::KEY_KP_5, Input::KEY_KP_6, Input::KEY_KP_7,
		Input::KEY_KP_8, Input::KEY_KP_9, Input::KEY_KP_MINUS,
		Input::KEY_KP_PERIOD, Input::KEY_KP_COMMA
	};
	m_value = 0;
	SendToText();
}

void sig::NumberEntry::OnKeyPress(int e)
{
	Entry::OnKeyPress(e);

	if (e == Input::KEY_RETURN || e == Input::KEY_KP_ENTER) {
		SendToValue();
	}
}

void sig::NumberEntry::OnBlur(MouseEvent e)
{
	SendToValue();
}

void sig::NumberEntry::SendToText()
{
	m_text = ToString<float>(m_value);
}

void sig::NumberEntry::SendToValue()
{
	SetValue(ToNumber<float>(m_text));
	SendToText();
}

sig::NumberEntry* sig::NumberEntry::SetValue(const float& value)
{
	if (value != m_value) {
		this->m_value = value;
		if (m_changeCallback) {
			m_changeCallback(m_value);
		}
	}
	SendToText();
	
	return this;
}
