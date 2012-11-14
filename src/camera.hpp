#ifndef CAMERA_H
#define CAMERA_H
#include "sequence.hpp"

#include <GL/glu.h>

using namespace Workspace;

class Camera: public Item
{
public:
	Camera();


	GLfloat tx;
	GLfloat ty;
	GLfloat tz;

	GLfloat rx;
	GLfloat ry;
	GLfloat rz;
};

#endif // CAMERA_H
