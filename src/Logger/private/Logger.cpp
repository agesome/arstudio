#include <Logger.hpp>

Workspace::Repository::ptr Logger::repo;

Logger::Logger ()
{
}

Logger &
Logger::getInstance (void)
{
  static Logger instance;

  return instance;
}

void
Logger::setRepository (Workspace::Repository::ptr r)
{
  repo = r;
}

void
Logger::advanceFrame (void)
{
  current_frame++;
}

void
Logger::resetFrameCounter (void)
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
Logger::logPoint (cv::Point3d point)
{
  Point3d::ptr p = Point3d::make (point.x, point.y, point.z, 1, 1, 1);

  repo->addItem (p, current_frame, Item::POINT3D, "points");
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
Logger::logCamera (cv::Point3d p, double rx, double ry, double rz)
{
  Camera::ptr c = Camera::make ();

  c->tx = p.x; c->ty = p.y; c->tz = p.z;
  c->rx = rx; c->ry = ry; c->rz = rz;

  repo->addItem (c, current_frame, Item::CAMERA, "camera");
}

/**
 *      This method logs a cv::Mat containing an image as internal Bitmap
 *      \param m the image to be logged
 *      \param source name of the sequence to log to
 */

void
Logger::addImage (cv::Mat & m, std::string source)
{
  Bitmap::ptr map = Bitmap::make ();

  map->bitmap = m;
  repo->addItem (map, current_frame, Item::BITMAP, source);
}
