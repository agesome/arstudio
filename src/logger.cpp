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
	PointCloud::ptr result = PointCloud::make ();
	Point3d::ptr point;

	if (m.channels () == 1)
		cv::cvtColor (m, m, CV_GRAY2BGR);
	for (int i = 0; i < m.rows; i += 10)
		{
			for (int j = 0; j < m.cols; j += 10)
				{
					cv::Vec3b p = m.at<cv::Vec3b> (i, j);
					point = Point3d::make (j / (m.cols * 1.5), i / (m.cols * 1.5), 0, p[2] / 255.0, p[1] / 255.0, p[0] / 255.0);
					result->cloud.push_back (point);
				}
		}
	repo->addItem (result, current_frame, Item::PCLOUD, source);
}
