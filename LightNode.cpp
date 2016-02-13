#include "LightNode.h"
#include "SIG_OpenGL.h"
#include "SIG_Utilities.h"

int sig::LightNode::index = 0;

sig::LightNode::LightNode()
	:	Node()
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

void sig::LightNode::Update(float dt)
{
	Node::Update(dt);
	
	int light = GL_LIGHT0 + index;

	float diffuse[] = { m_color.r, m_color.g, m_color.b, m_color.a };
	float pos[] 	= { m_position.X(), m_position.Y(), m_z };

    glLightfv(light, GL_DIFFUSE, diffuse);
    glLightfv(light, GL_POSITION, pos);
}
