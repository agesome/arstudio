#include <point3d.hpp>

Point3d::Point3d()
{

}

Point3d::Point3d(GLfloat cx,GLfloat cy,GLfloat cz,GLfloat cr,GLfloat cg,GLfloat cb)
{
	x=cx;
	y=cy;
	z=cz;
	r=cr;
	g=cg;
	b=cb;
}
