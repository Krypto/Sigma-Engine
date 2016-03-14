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

		bool IsShowingValue() const { return m_showValue; }
		void SetShowValue(bool showValue) { this->m_showValue = showValue; }

		void SetIncrement(float increment) { this->m_increment = increment; }
		float GetIncrement() const { return m_increment; }

		void SetMin(float _min) { this->m_min = _min; }
		void SetMax(float _max) { this->m_max = _max; }
		float GetMax() const { return m_max; }
		float GetMin() const { return m_min; }

		float GetValue() const { return m_value; }
		void SetValue(float value);
		
		void SetOrientation(int orientation) { this->m_orientation = orientation; }
		int GetOrientation() const { return m_orientation; }
		
		void SetSuffix(const string& suffix) { this->m_suffix = suffix; }
		const string& GetSuffix() const { return m_suffix; }
		
		Color& GetBarColor() { return m_barColor; }
		void SetBarColor(const Color& col) { m_barColor = col; }

		void SetCallback(function<void()> callback) { m_changeCallback = callback; }

		WIDGET_TYPE(WType::NUMBER_RANGE)
	private:
		float m_min, m_max, m_value;
		float m_x, m_increment;
		bool m_showValue, drag;
		int m_orientation;
		function<void()> m_changeCallback;
		string m_suffix;
		Color m_barColor;
	};

}

#endif // SIGMA_GUI_NUMBER_RANGE
