#include "Camera2D.h"
#include "SIG_OpenGL.h"
#include "SIG_Utilities.h"

sig::Camera2D::Camera2D()
	:	m_zoom(1.0f),
		m_zNear(-1.0f),
		m_zFar(1.0f)
{
}

void sig::Camera2D::ApplyTransformation(int w, int h)
{
	m_origin = Vector2(w / 2.0f, h / 2.0f);
	m_scale = Vector2(m_zoom);
	m_size = Vector2(1);
	
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, h, 0, m_zNear, m_zFar);
    glMatrixMode(GL_MODELVIEW);
	
	glLoadIdentity();
	
	glTranslatef(m_origin.X(), m_origin.Y(), 0);
	glRotatef(-ToDegrees(m_rotation), 0, 0, 1);
	glScalef(m_zoom, m_zoom, 1);
	glTranslatef(-m_position.X(), -m_position.Y(), 0);

}
