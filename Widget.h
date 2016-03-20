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
	
	enum class DockDirection {
		DOCK_LEFT,
		DOCK_TOP,
		DOCK_BOTTOM,
		DOCK_RIGHT,
		DOCK_FILL,
		DOCK_NONE
	};

	class GUI;
	class Widget
	{
		friend class GUI;
		friend class Box;
		friend class ScrollArea;
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
		
		const Color& GetBackColor() const { return m_backColor; }
		void SetBackColor(const Color& backColor) { this->m_backColor = backColor; }

		Rect GetBounds();
		void SetBounds(const Rect& bounds) { this->m_bounds = bounds; }

		const Color& GetTextColor() const { return m_textColor; }
		void SetTextColor(const Color& textColor) { this->m_textColor = textColor; }

		Widget* GetParent() { return m_parent; }

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
		
		DockDirection GetDockDirection() const { return m_dockDir; }
		void SetDockDirection(const DockDirection& dock) { m_dockDir = dock; }

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
		DockDirection m_dockDir;
		
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
