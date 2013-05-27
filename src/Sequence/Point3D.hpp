#ifndef POINT3D_H
#define POINT3D_H

#include <cassert>

#include <QColor>
#include <QVector3D>

#include <Sequence.hpp>

namespace arstudio {
class Point3D : public Item
{
  Q_OBJECT
  Q_PROPERTY (QVector3D position READ position)
  Q_PROPERTY (QColor color READ color)
public:
  typedef QSharedPointer<Point3D> ptr;

  Point3D (const QVector3D & position, const QColor & color)
  {
    position_ = position;
    color_    = color;
  }

  Point3D () = default;

  static ptr
  make (const QVector3D & position,
        const QColor & color)
  {
    return ptr (new Point3D (position, color));
  }

  const QVector3D
  position (void)
  {
    return position_;
  }
  const QColor
  color (void)
  {
    return color_;
  }
private:
  QVector3D position_;
  QColor    color_;
};
}

#endif // POINT3D_H
