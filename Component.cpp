#include "Component.h"
#include "SIG_OpenGL.h"
#include "Node.h"

sig::Component::Component(Node* owner)
{
	m_owner = owner;
	m_initialized = false;
}

sig::Component::~Component()
{
}
