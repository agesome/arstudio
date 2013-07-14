#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>

#include <Sequence.hpp>
#include <repository.pb.h>

namespace ap = arstudio_protobuf;

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

  inline void
  serialize (ap::Camera * c)
  {
    ap::Vector3D * v = c->mutable_position ();

    v->set_x (m_position.x ());
    v->set_y (m_position.y ());
    v->set_z (m_position.z ());

    v = c->mutable_rotation ();
    v->set_x (m_rotation.x ());
    v->set_y (m_rotation.y ());
    v->set_z (m_rotation.z ());
  }

  static inline Item::ptr
  deserialize (const ap::Item & i)
  {
    ap::Camera c = i.GetExtension (ap::Camera::camera);
    QVector3D  position (c.position ().x (), c.position ().y (),
                         c.position ().z ());
    QVector3D rotation (c.rotation ().x (), c.rotation ().y (),
                        c.rotation ().z ());

    return Camera::make (position, rotation);
  }

private:
  QVector3D m_position;
  QVector3D m_rotation;
};
}

#endif // CAMERA_H
