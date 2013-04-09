#ifndef ADJUSTER_HPP
#define ADJUSTER_HPP

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

#include <Logger.hpp>
#include <Config.hpp>
#include <AlgoInterface.hpp>

using namespace cv;

class ImageAdjuster
{
public:
	virtual Mat adjust (Mat & image) = 0;
	virtual ~ImageAdjuster()
	{
	};
};

class BrightnessAdjuster : public ImageAdjuster
{
public:
	double brightness;

	Mat adjust (Mat & image);
};

class ContrastAdjuster : public ImageAdjuster
{
public:
	double contrast;

	Mat adjust (Mat & image);
};

class ImageAdjusterAlgorithm : public IAbstractAlgorithm
{
public:
	std::string type;

	ImageAdjusterAlgorithm (Config::ptr config);
	~ImageAdjusterAlgorithm ();

	bool create ();
	bool run (Mat &, Mat &);

private:
	ImageAdjuster * adjuster = nullptr;
};
#endif // ADJUSTER_HPP
