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
	class Image;
	class ListView;
	class ListViewItem;
	
	using namespace math;
	class GUI
	{
		friend class Widget;
	public:
		GUI();
		~GUI();
		
		void AddWidget(Widget *w);

		void AddLabel(const string& text);
		Button* AddButton(const string& text);
		void AddSeparator();
		void AddParam(const string& text, int *value, int vmin, int vmax);
		void AddParam(const string& text, float *value, float vmin, float vmax, float increment=0.1f);
		void AddParam(const string& text, Color *value);
		void AddParam(const string& text, string *value, bool masked=false);
		void AddParam(const string& text, bool *value);
		void AddParam(const string& text, float *value);
		void AddParam(const string& text, Vector2 *value);
		Image* AddParam(const string& text, Texture2D *img);
		ListView* AddParam(const string& text, int *selected);

		void BeginBox(const Rect& bounds);
		void EndBox();

		Widget* GetFocused() { return m_focused; }
		Box* GetCurrentBox() { return m_currentBox; }
		
		void Initialize();
		void Update(float dt);
		void Render();
		
		static Texture2D *DEFAULT_FONT;
	protected:
		static int zindex;

		bool m_focusblur, m_enterleave;
		bool m_clickHandle, m_keyHandle, handled;
		Widget *m_focused;
		WidgetList m_widgets;
		vector<Box*> m_boxStack;
		Box *m_currentBox, *m_prevBox;
		int box_id;
	};

}

#endif // SIGMA_GUI
