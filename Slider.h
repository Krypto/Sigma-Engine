#ifndef SIGMA_GUI_NUMBER_RANGE
#define SIGMA_GUI_NUMBER_RANGE

#include "Label.h"
#include <functional>

namespace sig
{

	class Slider : public Label
	{
	public:
		enum {
			VERTICAL = 2,
			HORIZONTAL = 4
		};
		
		Slider();
		
		void OnMouseUp(MouseEvent e);
		void OnMouseDown(MouseEvent e);
		void OnMouseMove(MouseEvent e);
		void Render();
		
		Slider* SetIncrement(float increment) {this->m_increment = increment; return this;}
		Slider* SetMin(float _min) {this->m_min = _min; return this;}
		Slider* SetMax(float _max) {this->m_max = _max; return this;}
		Slider* SetShowValue(bool showValue) {this->m_showValue = showValue; return this;}
		Slider* SetValue(float value) ;
		float GetIncrement() const {return m_increment;}
		float GetMax() const {return m_max;}
		float GetMin() const {return m_min;}
		bool IsShowValue() const {return m_showValue;}
		float GetValue() const {return m_value;}
		
		Slider* SetOrientation(int orientation) {this->m_orientation = orientation; return this;}
		int GetOrientation() const {return m_orientation;}
		
		Slider* SetPrefix(const string& prefix) {this->m_prefix = prefix; return this;}
		const string& GetPrefix() const {return m_prefix;}
		
		void SetCallback(function<void()> callback) { m_changeCallback = callback; }
	private:
		float m_min, m_max, m_value;
		float m_x, m_increment;
		bool m_showValue, drag;
		int m_orientation;
		function<void()> m_changeCallback;
		string m_prefix;
	};

}

#endif // SIGMA_GUI_NUMBER_RANGE
