#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glu.h>

#include <Sequence.hpp>

namespace arstudio {
class Camera : public Item
{
public:
  typedef std::shared_ptr<Camera> ptr;
  static ptr
  make (void)
  {
    return std::make_shared<Camera> ();
  }

  Camera ()
  {
  };


  GLfloat tx;
  GLfloat ty;
  GLfloat tz;

  GLfloat rx;
  GLfloat ry;
  GLfloat rz;
};
}

#endif // CAMERA_H
