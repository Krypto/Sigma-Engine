#ifndef SIGMA_GUI_ENTRY
#define SIGMA_GUI_ENTRY

#include "Label.h"
#include "SIG_Utilities.h"

#include <vector>
#include <string>
#include <functional>
using namespace std;

namespace sig
{

	class Entry : public Label
	{
	public:
		Entry();
		
		void OnMouseDown(MouseEvent e);
		void OnKeyPress(int e);
		
		void SetText(const string& text);

		bool IsMasked() const { return m_masked; }
		void SetMasked(bool masked) { this->m_masked = masked; }

		void SetCallback(function<void()> callback) { m_changeCallback = callback; }

		void Render();
		void Update(float dt);

		WIDGET_TYPE(WType::ENTRY)
	protected:
		bool m_masked;
		vector<int> accepted_keys;
		u32 m_caretx;
		float m_btime;
		bool m_blink;
		function<void()> m_changeCallback;
	};

}

#endif // SIGMA_GUI_ENTRY
