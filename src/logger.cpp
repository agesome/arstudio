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
Logger::addImage (cv::Mat & m, std::string source)
{
	Bitmap::ptr map = Bitmap::make ();

	map->bitmap = m;
	repo->addItem (map, current_frame, Item::BITMAP, source);
}
