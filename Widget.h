#ifndef SIGMA_WIDGET
#define SIGMA_WIDGET

#include "Rect.h"
#include "Color.h"
#include "Vector.h"
#include "SIG_Utilities.h"
#include "Texture2D.h"

#include <string>
#include <vector>
#include <map>
using namespace std;

namespace sig
{
	using namespace math;
	
	typedef struct {
		Vector2 position;
		int button;
		void *hitWidget;
	} MouseEvent;
	
	typedef struct {
		int key;
	} KeyboardEvent;
	
	class GUI;
	class Widget
	{
		friend class GUI;
		friend class Box;
	public:
		enum WType {
			NUMBER,
			STRING,
			BOOLEAN,
			NUMBER_RANGE,
			WINDOW,
			BOX,
			BUTTON,
			EMPTY
		};
		
		Widget();
		virtual ~Widget();
		
		Widget* SetBackColor(const Color& backColor) {this->m_backColor = backColor; return this;}
		Widget* SetBounds(const Rect& bounds) {this->m_bounds = bounds; return this;}
		Widget* SetName(const string& name) {this->m_name = name; return this;}
		Widget* SetTextColor(const Color& textColor) {this->m_textColor = textColor; return this;}
		const Color& GetBackColor() const {return m_backColor;}
		Rect GetBounds();
		const string& GetName() const {return m_name;}
		Widget* GetParent() {return m_parent;}
		const Color& GetTextColor() const {return m_textColor;}
		
		void SetWidth(float w) {
			Rect b = GetBounds();
			b.width = w;
			SetBounds(b);
		}
		
		void SetHeight(float h) {
			Rect b = GetBounds();
			b.height = h;
			SetBounds(b);
		}
		
		bool IsActive() const { return m_active; }
		Widget* SetActive(bool active) { m_active = active; return this; }
		
		bool IsFocused() const { return m_focused; }
		
		virtual WType GetType() { return WType::EMPTY; }
		virtual bool IsSensitive() { return true; }
		
		GUI* GetGUIManager() { return m_guiManager; }
		
		virtual void OnMouseUp		(MouseEvent e) {}
		virtual void OnMouseDown	(MouseEvent e) {}
		virtual void OnMouseMove	(MouseEvent e) {}
		virtual void OnMouseLeave	() {}
		virtual void OnMouseEnter	() {}
		
		virtual void OnFocus		(MouseEvent e) {}
		virtual void OnBlur			(MouseEvent e) {}
		
		virtual void OnKeyPress		(int e) { }
		
		virtual void Update(float dt);
		virtual void Render();
	protected:
		int m_zindex;
		string m_name;
		
		Rect m_bounds;
		
		Color m_textColor;
		Color m_backColor;
		
		bool m_active;
		bool m_focused;
		
		Texture2D *m_font;
		
		Widget *m_parent;
		GUI *m_guiManager;
		
		int l_order;
		bool enter, focus, click, key;
	};

}

#endif // SIGMA_WIDGET
