#include <point3d.hpp>

Point3d::Point3d()
{
}

Point3d::Point3d(GLfloat cx, GLfloat cy, GLfloat cz, GLfloat cr, GLfloat cg, GLfloat cb)
{
	x = cx;
	y = cy;
	z = cz;
	r = cr;
	g = cg;
	b = cb;
}

Point3d::ptr
Point3d::make (GLfloat cx, GLfloat cy, GLfloat cz, GLfloat cr, GLfloat cg, GLfloat cb)
{
	return boost::make_shared<Point3d> (cx, cy, cz, cr, cg, cb);
}

Point3d::ptr
Point3d::make ()
{
	return boost::make_shared<Point3d> ();
}
