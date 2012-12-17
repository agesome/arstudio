#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <logger.hpp>
#include <config.hpp>
#include <iostream>

using namespace cv;

extern "C"
{
extern const char _module_desc[] = "Brightness";
extern const char _module_name[] = "brightness";

double beta = 80;

bool _module_init (void)
{
	Config & c = Config::getInstance ();

	c.addProperty ("algo.brightness.param.beta", beta);

	return true;
}

void _module_reconfigure (void)
{
	Config & c = Config::getInstance ();

	beta = c.getProperty ("algo.brightness.param.beta");
}

bool _module_process (Mat & image, Mat & dmap)
{
	Mat result = Mat::zeros (image.size (), image.type ());

	for (int y = 0; y < image.rows; y++)
		{
			for (int x = 0; x < image.cols; x++)
				{
					Vec3b src = image.at<Vec3b> (y, x);
					Vec3b r;
					r[0] = saturate_cast<uchar> (src[0] + beta);
					r[1] = saturate_cast<uchar> (src[1] + beta);
					r[2] = saturate_cast<uchar> (src[2] + beta);
					result.at<Vec3b> (y, x) = r;
				}
		}

	Logger & l = Logger::getInstance ();
	l.addImage (result, _module_name);
	return true;
}
}