#ifndef SIGMA_GUI
#define SIGMA_GUI

#include "Texture2D.h"
#include "Widget.h"
#include "IUR.h"
#include "Vector.h"

#include <vector>
#include <functional>
using namespace std;

namespace sig
{
	class Boolean;
	class NumberEntry;
	class Entry;
	class Label;
	class Button;
	class Box;
	class Slider;
	
	using namespace math;
	class GUI
	{
		friend class Widget;
	public:
		GUI();
		~GUI();
		
		Widget* CreateWidget(const string &name, Widget *w);
		Label* CreateLabel(const string &name, const string &text, float font_scale=0.27f, float char_spacing=-22.0f);
		Button* CreateButton(const string &name, const string &text, float font_scale=0.27f, float char_spacing=-22.0f);
		Entry* CreateEntry(const string &name, const string &text, float font_scale=0.27f, float char_spacing=-26.0f);
		NumberEntry* CreateNumberEntry(const string &name, float font_scale=0.27f, float char_spacing=-26.0f);
		Slider* CreateSlider(const string &name, float _min, float _max, float font_scale=0.27f, float char_spacing=-26.0f);
		Boolean* CreateBoolean(const string &name, float font_scale=0.27f, float char_spacing=-26.0f);
		Box* CreateBox(const string &name);
		
		Box* BeginBox(const Rect &rect);
		void RangeProperty(const string &title, function<float()> getter, function<void(float)> setter, float _min, float _max);
		void VectorProperty(const string &title, function<Vector2()> getter, function<void(Vector2)> setter);
		void EndBox();
		
		Widget* GetFocused() {return m_focused;}
		
		void Initialize();
		void Update(float dt);
		void Render();
		
		static Texture2D *DEFAULT_FONT;
	protected:
		static int zindex;
		static bool WidgetComparator(Widget *a, Widget *b);
		
		bool m_focusblur, m_enterleave;
		bool m_clickHandle, m_keyHandle, handled;
		Widget *m_focused;
		vector<Widget*> m_widgets;
		Box *m_currentBox;
		int pid;
	};

}

#endif // SIGMA_GUI
