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
		
		Label* SetText(const string& text) {this->m_text = text; return this;}
		const string& GetText() const {return m_text;}
		Label* SetCharSpacing(float charSpacing) {this->m_charSpacing = charSpacing; return this;}
		Label* SetFontScale(float fontScale) {this->m_fontScale = fontScale; return this;}
		float GetCharSpacing() const {return m_charSpacing;}
		float GetFontScale() const {return m_fontScale;}
		Label* SetAlign(int align) {this->m_align = align; return this;}
		int GetAlign() const {return m_align;}
		Label* SetTextMargin(float textMargin) {this->m_textMargin = textMargin; return this;}
		float GetTextMargin() const {return m_textMargin;}
		
		Vector2 Measure(const string& text);
		void Fit() {
			Vector2 m = Measure(m_text);
			m_bounds.width = m.X();
			m_bounds.height = m.Y()+4;
		}
		
		void Render();
	protected:
		string m_text;
		float m_fontScale;
		float m_charSpacing;
		float m_textMargin;
		int m_align;
	};

}

#endif // SIGMA_GUI_LABEL
