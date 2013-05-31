#ifndef POINT3D_H
#define POINT3D_H

#include <cassert>

#include <QColor>
#include <QVector3D>

#include <Sequence.hpp>

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
  position (void)
  {
    return m_position;
  }
  const QColor
  color (void)
  {
    return m_color;
  }
private:
  QVector3D m_position;
  QColor    m_color;
};
}

#endif // POINT3D_H
