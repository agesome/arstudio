#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>

#include <Sequence.hpp>

namespace arstudio {
class Camera : public Item
{
  Q_OBJECT
  Q_PROPERTY (QVector3D position READ position)
  Q_PROPERTY (QVector3D rotation READ rotation)
public:
  typedef QSharedPointer<Camera> ptr;
  static ptr
  make (const QVector3D & position, const QVector3D & rotation)
  {
    return ptr (new Camera (position, rotation));
  }

  Camera (const QVector3D & position, const QVector3D & rotation)
  {
    position_ = position;
    rotation_ = rotation;
  }

  Camera () = default;

  const QVector3D
  position (void)
  {
    return position_;
  }
  const QVector3D
  rotation (void)
  {
    return rotation_;
  }

private:
  QVector3D position_;
  QVector3D rotation_;
};
}

#endif // CAMERA_H
