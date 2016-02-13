#ifndef SIGMA_EDITOR
#define SIGMA_EDITOR

#include "imgui.h"
#include "Component.h"

namespace sig
{

	class Editor
	{
	public:
		virtual void GenerateEditor(Component *comp) {}
	};

}

#endif // SIGMA_EDITOR
