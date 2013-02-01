#include "adjuster.hpp"

using namespace cv;

Mat BrightnessAdjuster::adjust (Mat & image)
{
	Mat result = Mat::zeros (image.size (), image.type ());

	for (int y = 0; y < image.rows; y++)
		{
			for (int x = 0; x < image.cols; x++)
				{
					Vec3b src = image.at<Vec3b> (y, x);
					Vec3b r;
					r[0] = saturate_cast<uchar> (src[0] + brightness);
					r[1] = saturate_cast<uchar> (src[1] + brightness);
					r[2] = saturate_cast<uchar> (src[2] + brightness);
					result.at<Vec3b> (y, x) = r;
				}
		}
	return result;
}

Mat ContrastAdjuster::adjust (Mat & image)
{
	Mat result = Mat::zeros (image.size (), image.type ());

	for (int y = 0; y < image.rows; y++)
		{
			for (int x = 0; x < image.cols; x++)
				{
					Vec3b src = image.at<Vec3b> (y, x);
					Vec3b r;
					r[0] = saturate_cast<uchar> (src[0] * contrast);
					r[1] = saturate_cast<uchar> (src[1] * contrast);
					r[2] = saturate_cast<uchar> (src[2] * contrast);
					result.at<Vec3b> (y, x) = r;
				}
		}
	return result;
}

ImageAdjusterAlgorithm::ImageAdjusterAlgorithm (Config * config)
	: IAbstractAlgorithm (config)
{
	type = config->get<std::string> ("algorithm.type");
}

ImageAdjusterAlgorithm::~ImageAdjusterAlgorithm ()
{
}

bool ImageAdjusterAlgorithm::create ()
{
	std::string path = "algorithm.runtime-parameters.";

	if (type == "brightness")
		{
			BrightnessAdjuster * v = new BrightnessAdjuster ();
			v->brightness = config->get<double>(path + type);
			adjuster = v;
		}
	else
		{
			ContrastAdjuster * v = new ContrastAdjuster ();
			v->contrast = config->get<double>(path + type);
			adjuster = v;
		}

	return true;
}

bool ImageAdjusterAlgorithm::run (Mat & image, Mat & dmap)
{
	Mat result = adjuster->adjust (image);

	Logger & l = Logger::getInstance ();

	l.addImage (result, type);
	return true;
}
