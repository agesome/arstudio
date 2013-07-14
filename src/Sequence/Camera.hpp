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
  make (const QVector3D & position, const QVector3D & rotation);

  Camera (const QVector3D & position, const QVector3D & rotation);

  const QVector3D
  position ();
  const QVector3D
  rotation ();

  void
  serialize (ap::Camera * c);

  static Item::ptr
  deserialize (const ap::Item & i);

private:
  QVector3D m_position;
  QVector3D m_rotation;
};
}

#endif // CAMERA_H
