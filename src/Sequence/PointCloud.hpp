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

  static ptr
  make ();

  int
  point_count ();

  QVector3D
  point_at (int i);

  QColor
  point_color_at (int i);

  void
  append_point (const QVector3D & p, const QColor & c = QColor (Qt::white));
private:
  QVector<QVector3D> m_points;
  QVector<QColor>    m_colors;
};
}

#endif // PCLOUD_H
