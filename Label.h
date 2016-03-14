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
		
		virtual void SetText(const string& text) { this->m_text = text; }
		const string& GetText() const { return m_text; }

		void SetCharSpacing(float charSpacing) { this->m_charSpacing = charSpacing; }
		void SetFontScale(float fontScale) { this->m_fontScale = fontScale; }
		float GetCharSpacing() const { return m_charSpacing; }
		float GetFontScale() const { return m_fontScale; }

		void SetAlign(int align) { this->m_align = align; }
		int GetAlign() const { return m_align; }

		void SetTextMargin(float textMargin) { this->m_textMargin = textMargin; }
		float GetTextMargin() const { return m_textMargin; }
		
		Vector2 Measure(const string& text);
		void Fit(int padding = 1) {
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
