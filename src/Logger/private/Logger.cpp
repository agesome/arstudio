#include <Logger.hpp>

namespace arstudio {
Repository::ptr Logger::repo;
Logger          Logger::instance_;

Logger &
Logger::instance (void)
{
  return instance_;
}

void
Logger::set_repository (Repository::ptr r)
{
  repo = r;
}

void
Logger::advance_frame (void)
{
  current_frame++;
}

void
Logger::reset_frame_counter (void)
{
  current_frame = 1;
}

/**
 *      This method logs an OpenCV Point3d, converting it to the internally
 * used Point3d
 *
 *      \param point the point to be logged
 */

void
Logger::log_point (const cv::Point3d & point, const std::string & name)
{
  Point3D::ptr p = Point3D::make (QVector3D (point.x, point.y, point.z),
                                  QColor (255, 255, 255));

  repo->add_item (p, current_frame, Sequence::POINT3D,
                  QString::fromStdString (name));
}

void
Logger::log_point (const cv::Point3d & point)
{
  this->log_point (point, "points");
}

/**
 *      This method logs a Camera
 *
 *      \param p position
 *      \param rx rotation on the X axis
 *      \param ry rotation on the Y axis
 *      \param rz rotation on the Z axis
 */

void
Logger::log_camera (const cv::Point3d & pos, const cv::Point3d & r,
                    const std::string & name)
{
  Camera::ptr c = Camera::make (QVector3D (pos.x, pos.y, pos.z),
                                QVector3D (r.x, r.y, r.z));

  repo->add_item (c, current_frame, Sequence::CAMERA,
                  QString::fromStdString (name));
}

void
Logger::log_camera (const cv::Point3d & pos, const cv::Point3d & r)
{
  this->log_camera (pos, r, "camera");
}

/**
 *      This method logs a cv::Mat containing an image as internal Bitmap
 *      \param m the image to be logged
 *      \param source name of the sequence to log to
 */

void
Logger::log_image (const cv::Mat & m, const std::string & name)
{
  Bitmap::ptr map = Bitmap::make ();

  map->bitmap = m;
  repo->add_item (map, current_frame, Sequence::BITMAP,
                  QString::fromStdString (name));
}

void
Logger::log_image (const cv::Mat & m)
{
  this->log_image (m, "image");
}
}
