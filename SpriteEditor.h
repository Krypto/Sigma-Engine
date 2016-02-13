#ifndef SIGMA_SPRITE_EDITOR
#define SIGMA_SPRITE_EDITOR

#include "Editor.h"
#include "Sprite.h"

namespace sig
{

	class SpriteEditor : public Editor
	{
	public:
		void GenerateEditor(Component* comp);
	};

}

#endif // SIGMA_SPRITE_EDITOR
