#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>

#include <Sequence.hpp>

namespace arstudio {
class Camera : public Item
{
public:
  typedef QSharedPointer<Camera> ptr;
  static ptr
  make (const QVector3D & position, const QVector3D & rotation)
  {
    return ptr (new Camera (position, rotation));
  }

  Camera (const QVector3D & position, const QVector3D & rotation)
  {
    m_position = position;
    m_rotation = rotation;
  }

  const QVector3D
  position ()
  {
    return m_position;
  }
  const QVector3D
  rotation ()
  {
    return m_rotation;
  }

private:
  QVector3D m_position;
  QVector3D m_rotation;
};
}

#endif // CAMERA_H
