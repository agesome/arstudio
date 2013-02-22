#ifndef LOGGER_H
#define LOGGER_H

#include <repository.hpp>
#include <pcloud.hpp>
#include <bitmap.hpp>
#include <point3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/nonfree/features2d.hpp>


class Logger
{
public:
	static Logger & getInstance (void);
	static void setRepository (const Workspace::Repository::ptr);

	void addImage (cv::Mat &, std::string);
	void logPoint (cv::Point3d point);
	void advanceFrame (void);
private:
	Logger ();
	Logger (const Logger &);
	void operator= (const Logger &);

	static Workspace::Repository::ptr repo;
	unsigned int current_frame = 1;
};
#endif // LOGGER_H
