#ifndef POINT3D_H
#define POINT3D_H
#include <GL/glu.h>
#include <sequence.hpp>

using namespace Workspace;

class Point3d : public Item
{
public:
	Point3d();

	Point3d(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);

public:
	GLfloat x;
	GLfloat y;
	GLfloat z;

	GLfloat r;
	GLfloat g;
	GLfloat b;
};

#endif // POINT3D_H
