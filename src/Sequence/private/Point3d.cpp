#include <Point3D.hpp>

namespace arstudio {
Point3D::Point3D ()
{
}

Point3D::Point3D (GLfloat cx,
                  GLfloat cy,
                  GLfloat cz,
                  GLfloat cr,
                  GLfloat cg,
                  GLfloat cb)
{
  x = cx;
  y = cy;
  z = cz;
  r = cr;
  g = cg;
  b = cb;
}

Point3D::ptr
Point3D::make (GLfloat cx,
               GLfloat cy,
               GLfloat cz,
               GLfloat cr,
               GLfloat cg,
               GLfloat cb)
{
  return std::make_shared<Point3D> (cx, cy, cz, cr, cg, cb);
}

Point3D::ptr
Point3D::make ()
{
  return std::make_shared<Point3D> ();
}
}
