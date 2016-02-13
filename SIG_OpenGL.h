#ifndef SIGMA_OPENGL
#define SIGMA_OPENGL

#define GLEW_STATIC
#include <GL/glew.h>

#ifdef __APPLE__
#	include <OpenGL/gl.h>
#	include <OpenGL/glu.h>
#	include <GLUT/glut.h>
#else
#	ifdef _WIN32
#		include <windows.h>
#	endif
#	include <GL/gl.h>
#	include <GL/glu.h>
#endif

#endif