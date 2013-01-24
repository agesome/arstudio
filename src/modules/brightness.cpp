#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <logger.hpp>

using namespace cv;

extern "C"
{
double beta;

bool process (Mat & image, Mat & dmap)
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
	l.addImage (result, "brightness");
	return true;
}
}