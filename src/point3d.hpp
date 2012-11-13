#ifndef POINT3D_H
#define POINT3D_H
#include <GL/glu.h>
#include <sequence.hpp>

using namespace Workspace;

class Point3d : public Item
{
public:
	Point3d();

	<<<<<<< HEAD
	Point3d(GLfloat,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat);

public:
	GLfloat x;
	GLfloat y;
	GLfloat z;

	=======
  private:
	  GLfloat x;
	GLfloat y;
	GLfloat z;

	>>>>>>> d2aab5d55ba7046fd6ba950e7cb31dcf34284ff6
	GLfloat r;
	GLfloat g;
	GLfloat b;


};

#endif // POINT3D_H
