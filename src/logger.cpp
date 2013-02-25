#include <logger.hpp>

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
Logger::logPoint (cv::Point3d point)
{
	Point3d::ptr p = Point3d::make (point.x, point.y, point.z, 1, 1, 1);

	repo->addItem (p, current_frame, Item::POINT3D, "points");
}

void
Logger::addImage (cv::Mat & m, std::string source)
{
	Bitmap::ptr map = Bitmap::make ();

	map->bitmap = m;
	repo->addItem (map, current_frame, Item::BITMAP, source);
}
