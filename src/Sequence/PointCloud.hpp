#ifndef PCLOUD_H
#define PCLOUD_H

#include <QVector3D>
#include <QVector>
#include <QColor>

#include <Sequence.hpp>
#include <repository.pb.h>

namespace ap = arstudio_protobuf;

namespace arstudio {
class PointCloud : public Item
{
public:
  typedef QSharedPointer<PointCloud> ptr;

  static ptr
  make ()
  {
    return ptr (new PointCloud ());
  }

  int
  point_count ()
  {
    return m_points.size ();
  }

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

  inline void
  serialize (ap::PointCloud * pc)
  {
    ap::Vector3D * v;
    ap::Color    * col;

    for (const QVector3D & p : m_points)
      {
        v = pc->add_points ();
        v->set_x (p.x ());
        v->set_y (p.y ());
        v->set_z (p.z ());
      }
    for (const QColor & c : m_colors)
      {
        col = pc->add_colors ();
        col->set_r (c.red ());
        col->set_g (c.green ());
        col->set_b (c.blue ());
      }
  }

  static inline Item::ptr
  deserialize (const ap::Item & i)
  {
    ap::PointCloud  pc = i.GetExtension (ap::PointCloud::point_cloud);
    ap::Vector3D    v;
    ap::Color       c;
    PointCloud::ptr p = PointCloud::make ();

    /*
     * Here we assume pc.points_size() == pc.colors_size(); otherwise,
     * things are very broken
     */
    for (int i = 0; i < pc.points_size (); i++)
      {
        v = pc.points (i);
        c = pc.colors (i);
        p->append_point (QVector3D (v.x (), v.y (), v.z ()),
                         QColor::fromRgb (c.r (), c.g (), c.b ()));
      }
    return p;
  }

private:
  QVector<QVector3D> m_points;
  QVector<QColor>    m_colors;
};
}

#endif // PCLOUD_H
