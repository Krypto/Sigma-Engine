#ifndef SIGMA_ENGINE
#define SIGMA_ENGINE

#include "ResourceManager.h"
#include <string>

// Some macros to help you out ;)

// Resource manager instance
#define Resources ResourceManager::Instance()
// Get a Resource by name
#define GetResource(name) Resources->GetResource(name)
// Get a Resource (Data) by name and casts it into rtype
#define GetResourceData(rtype, name) (rtype) GetResource(name)->GetData()
#define GetResourceText(name) *((std::string*)GetResource(name)->GetData())
// Creates a new resource with the specified name, file name and ResourceType
#define NewResource(name, filename, rtype) Resources->AddResource(name, filename, rtype)

#include "Resource.h"

#include "SpriteBatch.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "TileMap.h"
#include "ParticleEmitter.h"

#include "Component.h"
#include "MessageNetwork.h"
#include "BaseGame.h"

#include "Node.h"
#include "LightNode.h"

#include "Vector.h"
#include "Matrix4.h"
#include "Quaternion.h"
#include "Rect.h"
#include "Color.h"

#include "Input.h"

#include "GUI.h"
#include "Entry.h"
#include "NumberEntry.h"
#include "Boolean.h"
#include "Slider.h"
#include "Button.h"
#include "Label.h"
#include "Box.h"
#include "Widget.h"

#include "LuaEngine.h"
#include "SIG_Utilities.h"
#include <cmath>

#endif // SIGMA_ENGINE
