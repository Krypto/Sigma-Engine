#ifndef SIGMA_ANIMATED_SPRITE_EDITOR
#define SIGMA_ANIMATED_SPRITE_EDITOR

#include "Editor.h"
#include "AnimatedSprite.h"

#include <vector>
#include <string>
using namespace std;

namespace sig
{

	class AnimatedSpriteEditor : public Editor
	{
	public:
		void GenerateEditor(Component* comp);
	};

}

#endif // SIGMA_ANIMATED_SPRITE_EDITOR
