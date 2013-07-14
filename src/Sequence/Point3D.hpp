#ifndef POINT3D_H
#define POINT3D_H

#include <QColor>
#include <QVector3D>

#include <Sequence.hpp>
#include <repository.pb.h>

namespace ap = arstudio_protobuf;

namespace arstudio {
class Point3D : public Item
{
public:
  typedef QSharedPointer<Point3D> ptr;

  Point3D (const QVector3D & position, const QColor & color)
  {
    m_position = position;
    m_color    = color;
  }

  static ptr
  make (const QVector3D & position,
        const QColor & color)
  {
    return ptr (new Point3D (position, color));
  }

  const QVector3D
  position ()
  {
    return m_position;
  }

  const QColor
  color ()
  {
    return m_color;
  }

  inline void
  serialize (ap::Point * p)
  {
    ap::Vector3D * v = p->mutable_position ();

    v->set_x (m_position.x ());
    v->set_y (m_position.y ());
    v->set_z (m_position.z ());
  }

  static inline Item::ptr
  deserialize (const ap::Item & i)
  {
    ap::Point p = i.GetExtension (ap::Point::point);
    QVector3D position (p.position ().x (), p.position ().y (),
                        p.position ().z ());
    QColor color = QColor::fromRgb (p.color ().r (),
                                    p.color ().g (), p.color ().b ());

    return Point3D::make (position, color);
  }


private:
  QVector3D m_position;
  QColor    m_color;
};
}

#endif // POINT3D_H
