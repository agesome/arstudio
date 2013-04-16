#ifndef ADJUSTER_HPP
#define ADJUSTER_HPP

#include <iostream>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <Logger.hpp>
#include <Config.hpp>
#include <AlgoInterface.hpp>

using namespace cv;

class ImageAdjuster
{
public:
	virtual Mat adjust (const Mat & image) = 0;
	virtual ~ImageAdjuster()
	{
	};
};

class BrightnessAdjuster : public ImageAdjuster
{
public:
	double brightness;

	Mat adjust (const Mat & image);
};

class ContrastAdjuster : public ImageAdjuster
{
public:
	double contrast;

	Mat adjust (const Mat & image);
};

class ImageAdjusterAlgorithm : public IAbstractAlgorithm
{
public:
	ImageAdjusterAlgorithm (Config::ptr config);
	~ImageAdjusterAlgorithm ();

	bool create (void);
	bool run (const Mat &, const Mat &);
	const std::string id_string (void);

	std::string type;
private:
	ImageAdjuster * adjuster = nullptr;
};
#endif // ADJUSTER_HPP
