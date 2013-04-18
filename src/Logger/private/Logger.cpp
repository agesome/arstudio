#include <Logger.hpp>

Workspace::Repository::ptr Logger::repo;
Logger                     Logger::instance_;

Logger::Logger ()
{
}

Logger &
Logger::instance (void)
{
  return instance_;
}

void
Logger::set_repository (Workspace::Repository::ptr r)
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
Logger::log_point (cv::Point3d point, const std::string & name)
{
  Point3d::ptr p = Point3d::make (point.x, point.y, point.z, 1, 1, 1);

  repo->addItem (p, current_frame, Item::POINT3D, name);
}

void
Logger::log_point (cv::Point3d point)
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
Logger::log_camera (cv::Point3d p, double rx, double ry, double rz,
                    const std::string & name)
{
  Camera::ptr c = Camera::make ();

  c->tx = p.x;
  c->ty = p.y;
  c->tz = p.z;
  c->rx = rx;
  c->ry = ry;
  c->rz = rz;

  repo->addItem (c, current_frame, Item::CAMERA, name);
}

void
Logger::log_camera (cv::Point3d p, double rx, double ry, double rz)
{
  this->log_camera (p, rx, ry, rz, "camera");
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
  repo->addItem (map, current_frame, Item::BITMAP, name);
}

void
Logger::log_image (const cv::Mat & m)
{
  this->log_image (m, "image");
}
