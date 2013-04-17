#ifndef POINT3D_H
#define POINT3D_H

#include <GL/glu.h>

#include <Sequence.hpp>

using namespace Workspace;

class Point3d : public Item
{
public:
	typedef std::shared_ptr<Point3d> ptr;

	static ptr make (void);
	static ptr make (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);

	Point3d ();
	Point3d (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);

	GLfloat x;
	GLfloat y;
	GLfloat z;

	GLfloat r;
	GLfloat g;
	GLfloat b;
};

#endif // POINT3D_H
