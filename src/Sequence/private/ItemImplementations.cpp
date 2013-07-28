#include <Bitmap.hpp>
#include <Camera.hpp>
#include <Point3D.hpp>
#include <PointCloud.hpp>

namespace arstudio {
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

/*
 * PointCloud
 */

PointCloud::PointCloud (const QVector<QVector3D> &points,
                        const QVector<QColor> &colors)
  : m_points (points)
  , m_colors (colors)
{
}

PointCloud::ptr
PointCloud::make ()
{
  return ptr (new PointCloud ());
}

PointCloud::ptr
PointCloud::make (const QVector<QVector3D> &points,
                  const QVector<QColor> &colors)
{
  return ptr (new PointCloud (points, colors));
}

int
PointCloud::point_count ()
{
  return m_points.size ();
}

QVector<QVector3D>
PointCloud::points ()
{
  return m_points;
}

QVector<QColor>
PointCloud::colors ()
{
  return m_colors;
}
}
