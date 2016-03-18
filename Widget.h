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

#define WIDGET_TYPE(type) virtual WType GetType() const { return type; }

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
		enum class WType {
			NUMBER,
			ENTRY,
			BOOLEAN,
			NUMBER_RANGE,
			BOX,
			BUTTON,
			LABEL,
			IMAGE,
			LIST,
			EMPTY
		};

		WIDGET_TYPE(WType::EMPTY)
		
		Widget();
		virtual ~Widget();
		
		void SetBackColor(const Color& backColor) {this->m_backColor = backColor;}
		void SetBounds(const Rect& bounds) {this->m_bounds = bounds;}
		void SetTextColor(const Color& textColor) {this->m_textColor = textColor;}

		const Color& GetBackColor() const {return m_backColor;}
		Rect GetBounds();
		Widget* GetParent() {return m_parent;}
		const Color& GetTextColor() const {return m_textColor;}
		int GetZIndex() const { return m_zindex; }
		
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
	typedef vector<Widget*> WidgetList;
}

#endif // SIGMA_WIDGET
