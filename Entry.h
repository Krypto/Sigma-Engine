#ifndef SIGMA_GUI_ENTRY
#define SIGMA_GUI_ENTRY

#include "Label.h"

#include <vector>
#include <string>
using namespace std;

namespace sig
{

	class Entry : public Label
	{
	public:
		Entry();
		
		void OnMouseDown(MouseEvent e);
		void OnKeyPress(int e);
		
		Entry* SetMasked(bool masked) {this->m_masked = masked; return this;}
		bool IsMasked() const {return m_masked;}
		
		WType GetType() { return WType::STRING; }
		
		void Render();
		void Update(float dt);
	protected:
		bool m_masked;
		vector<int> accepted_keys;
		int m_caretx;
		float m_btime;
		bool m_blink;
	};

}

#endif // SIGMA_GUI_ENTRY
