#include "LightNode.h"
#include "Node.h"
#include "SIG_OpenGL.h"
#include "SIG_Utilities.h"

int sig::Light::index = 0;

sig::Light::Light()
	:	Component()
{
	m_z = 1;
	m_color = Color::WHITE;
	
	glEnable(GL_LIGHT0 + index);
	index++;
	if (index > 7) {
		glDisable(GL_LIGHT0 + (index-1));
		index = 0;
		glEnable(GL_LIGHT0 + index);
	}
}

void sig::Light::Update(float dt)
{
	int light = GL_LIGHT0 + index;

	float diffuse[] = { m_color.r, m_color.g, m_color.b, m_color.a };
	float pos[] 	= { GetOwner()->GetPosition().X(),
						GetOwner()->GetPosition().Y(),
						m_z };

	glLightfv(light, GL_DIFFUSE,	diffuse);
	glLightfv(light, GL_POSITION,	pos);
}
