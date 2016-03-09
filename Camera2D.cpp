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
	glLoadMatrixf(&GetCameraMatrix()(0, 0));
}

sig::math::Matrix4 sig::Camera2D::GetCameraMatrix()
{
	Matrix4 t1 = math::Matrix4::MakeTranslation(Vector3(m_origin.X(), m_origin.Y(), 0));
	Matrix4 r = math::Matrix4::MakeRotation(-m_rotation, Vector3(0, 0, 1));
	Matrix4 s = math::Matrix4::MakeScale(Vector3(m_zoom, m_zoom, 0));
	Matrix4 t2 = math::Matrix4::MakeTranslation(Vector3(-m_position.X(), -m_position.Y(), 0));

	return t1 * r * s * t2;
}
