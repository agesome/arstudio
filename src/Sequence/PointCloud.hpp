#ifndef PCLOUD_H
#define PCLOUD_H

#include <QVector3D>
#include <QVector>
#include <QColor>

#include <Sequence.hpp>

namespace arstudio {
class PointCloud : public Item
{
public:
  typedef QSharedPointer<PointCloud> ptr;

  PointCloud (const QVector<QVector3D> & points,
              const QVector<QColor> & colors);
  PointCloud () = default;

  static ptr
  make ();

  static ptr
  make (const QVector<QVector3D> & points, const QVector<QColor> & colors);

  int
  point_count ();

  inline QVector3D
  point_at (int i)
  {
    return m_points.at (i);
  }

  inline QColor
  point_color_at (int i)
  {
    return m_colors.at (i);
  }

  inline void
  append_point (const QVector3D & p, const QColor & c = QColor (Qt::white))
  {
    m_points.append (p);
    m_colors.append (c);
  }

  QVector<QVector3D>
  points ();

  QVector<QColor>
  colors ();
private:
  QVector<QVector3D> m_points;
  QVector<QColor>    m_colors;
};
}

#endif // PCLOUD_H
