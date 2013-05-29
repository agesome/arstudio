#ifndef PCLOUD_H
#define PCLOUD_H

#include <QVector3D>
#include <QList>

#include <Sequence.hpp>

namespace arstudio {
class PointCloud : public Item
{
  Q_OBJECT
  Q_PROPERTY (QList<QVector3D> points READ points)
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

  PointCloud () = default;

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
