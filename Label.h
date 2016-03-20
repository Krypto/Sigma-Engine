#ifndef SIGMA_GUI_LABEL
#define SIGMA_GUI_LABEL

#include "Widget.h"
#include "Vector.h"

namespace sig
{
	using namespace math;
	class Label : public Widget
	{
	public:
		enum {
			ALIGN_LEFT = 0,
			ALIGN_CENTER = 1,
			ALIGN_RIGHT = 2
		};
		
		Label();
		
		const string& GetText() const { return m_text; }
		virtual void SetText(const string& text) { this->m_text = text; }

		float GetCharSpacing() const { return m_charSpacing; }
		void SetCharSpacing(float charSpacing) { this->m_charSpacing = charSpacing; }

		float GetFontScale() const { return m_fontScale; }
		void SetFontScale(float fontScale) { this->m_fontScale = fontScale; }

		int GetAlign() const { return m_align; }
		void SetAlign(int align) { this->m_align = align; }

		float GetTextMargin() const { return m_textMargin; }
		void SetTextMargin(float textMargin) { this->m_textMargin = textMargin; }

		Vector2 Measure(const string& text);
		void Fit(int padding = -2) {
			Vector2 m = Measure(m_text);
			m_bounds.width = m.X()+padding;
			m_bounds.height = m.Y()+padding;
		}
		
		void Render();

		WIDGET_TYPE(WType::LABEL)
	protected:
		string m_text;
		float m_fontScale;
		float m_charSpacing;
		float m_textMargin;
		int m_align;
	};

}

#endif // SIGMA_GUI_LABEL
