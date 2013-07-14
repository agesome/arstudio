#include <Bitmap.hpp>
#include <Camera.hpp>
#include <Point3D.hpp>
#include <PointCloud.hpp>

namespace arstudio {
/*
 * Utility type conversion functions, local to this file
 */

namespace {
inline void
qv2apv (const QVector3D & v, ap::Vector3D * apv)
{
  apv->set_x (v.x ());
  apv->set_y (v.y ());
  apv->set_z (v.z ());
}

inline QVector3D
apv2qv (const ap::Vector3D & v)
{
  return QVector3D (v.x (), v.y (), v.z ());
}

inline void
qc2apc (const QColor & c, ap::Color * apc)
{
  apc->set_r (c.red ());
  apc->set_g (c.green ());
  apc->set_b (c.blue ());
}

inline QColor
apc2qc (const ap::Color & c)
{
  return QColor::fromRgb (c.r (), c.g (), c.b ());
}
}

Bitmap::Bitmap (QImage p)
{
  m_image = p.copy ();
}

Bitmap::ptr
Bitmap::make (QImage &p)
{
  return ptr (new Bitmap (p));
}

QImage
Bitmap::get ()
{
  return m_image;
}

void
Bitmap::serialize (arstudio_protobuf::Bitmap * bm)
{
  QBuffer buf;

  buf.open (QBuffer::WriteOnly);
  m_image.save (&buf, "PNG");
  buf.close ();
  bm->set_allocated_data (new std::string (buf.data ().data (),
                                           buf.data ().size ()));
}

Item::ptr
Bitmap::deserialize (const arstudio_protobuf::Item &i)
{
  ap::Bitmap bm = i.GetExtension (ap::Bitmap::bitmap);
  QImage     img;

  img.loadFromData (reinterpret_cast<const uchar *> (bm.data ().c_str ()),
                    bm.data ().size (), "PNG");
  return Bitmap::make (img);
}


/*
 * Camera
 */

Camera::ptr
Camera::make (const QVector3D &position, const QVector3D &rotation)
{
  return ptr (new Camera (position, rotation));
}

Camera::Camera (const QVector3D &position, const QVector3D &rotation)
  : m_position (position),
  m_rotation (rotation)
{
}

const QVector3D
Camera::position ()
{
  return m_position;
}

const QVector3D
Camera::rotation ()
{
  return m_rotation;
}

void
Camera::serialize (arstudio_protobuf::Camera * c)
{
  ap::Vector3D * v = c->mutable_position ();

  qv2apv (m_position, v);

  v = c->mutable_rotation ();
  qv2apv (m_rotation, v);
}

Item::ptr
Camera::deserialize (const arstudio_protobuf::Item &i)
{
  ap::Camera c = i.GetExtension (ap::Camera::camera);

  return Camera::make (apv2qv (c.position ()), apv2qv (c.rotation ()));
}

/*
 * Point3D
 */

Point3D::Point3D (const QVector3D &position, const QColor &color)
  : m_position (position),
  m_color (color)
{
}

Point3D::ptr
Point3D::make (const QVector3D &position, const QColor &color)
{
  return ptr (new Point3D (position, color));
}

const QVector3D
Point3D::position ()
{
  return m_position;
}

const QColor
Point3D::color ()
{
  return m_color;
}

void
Point3D::serialize (arstudio_protobuf::Point * p)
{
  qv2apv (m_position, p->mutable_position ());
  qc2apc (m_color, p->mutable_color ());
}

Item::ptr
Point3D::deserialize (const arstudio_protobuf::Item &i)
{
  ap::Point p = i.GetExtension (ap::Point::point);

  return Point3D::make (apv2qv (p.position ()), apc2qc (p.color ()));
}

/*
 * PointCloud
 */

PointCloud::ptr
PointCloud::make ()
{
  return ptr (new PointCloud ());
}

int
PointCloud::point_count ()
{
  return m_points.size ();
}

inline QVector3D
PointCloud::point_at (int i)
{
  return m_points.at (i);
}

inline QColor
PointCloud::point_color_at (int i)
{
  return m_colors.at (i);
}

inline void
PointCloud::append_point (const QVector3D &p, const QColor &c)
{
  m_points.append (p);
  m_colors.append (c);
}

void
PointCloud::serialize (arstudio_protobuf::PointCloud * pc)
{
  for (const QVector3D & p : m_points)
    qv2apv (p, pc->add_points ());
  for (const QColor & c : m_colors)
    qc2apc (c, pc->add_colors ());
}

Item::ptr
PointCloud::deserialize (const arstudio_protobuf::Item &i)
{
  ap::PointCloud  pc = i.GetExtension (ap::PointCloud::point_cloud);
  PointCloud::ptr p  = PointCloud::make ();

  /*
   * Here we assume pc.points_size() == pc.colors_size(); otherwise,
   * things are very broken
   */
  for (int i = 0; i < pc.points_size (); i++)
    p->append_point (apv2qv (pc.points (i)), apc2qc (pc.colors (i)));
  return p;
}
}
