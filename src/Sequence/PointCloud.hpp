#ifndef PCLOUD_H
#define PCLOUD_H

#include <QVector3D>
#include <QList>

#include <Sequence.hpp>

namespace arstudio {
class PointCloud : public Item
{
public:
  typedef QSharedPointer<PointCloud> ptr;
  static ptr
  make (const QList<QVector3D> & points)
  {
    return ptr (new PointCloud (points));
  }

  PointCloud (const QList<QVector3D> & points)
  {
    m_points = points;
  }

  const QList<QVector3D> &
  points (void)
  {
    return m_points;
  }

private:
  QList<QVector3D> m_points;
};
}

#endif // PCLOUD_H
