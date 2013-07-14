#ifndef POINT3D_H
#define POINT3D_H

#include <QColor>
#include <QVector3D>

#include <Sequence.hpp>

namespace arstudio {
class Point3D : public Item
{
public:
  typedef QSharedPointer<Point3D> ptr;

  Point3D (const QVector3D & position, const QColor & color);

  static ptr
  make (const QVector3D & position,
        const QColor & color);

  const QVector3D
  position ();

  const QColor
  color ();

  void
  serialize (ap::Point * p);

  static Item::ptr
  deserialize (const ap::Item & i);


private:
  QVector3D m_position;
  QColor    m_color;
};
}

#endif // POINT3D_H
