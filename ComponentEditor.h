#ifndef SIGMA_COMPONENT_EDITOR
#define SIGMA_COMPONENT_EDITOR

#include "Editor.h"

namespace sig
{

	class ComponentEditor : public Editor
	{
	public:
		void GenerateEditor(Component *comp);
	};

}

#endif // SIGMA_COMPONENT_EDITOR
