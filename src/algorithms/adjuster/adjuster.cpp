#include "adjuster.hpp"

using namespace cv;

Mat
BrightnessAdjuster::adjust (const Mat & image)
{
  Mat result = Mat::zeros (image.size (), image.type ());

  for (int y = 0; y < image.rows; y++)
    for (int x = 0; x < image.cols; x++)
      {
        Vec3b src = image.at<Vec3b> (y, x);
        Vec3b r;
        r[0]                    = saturate_cast<uchar> (src[0] + brightness);
        r[1]                    = saturate_cast<uchar> (src[1] + brightness);
        r[2]                    = saturate_cast<uchar> (src[2] + brightness);
        result.at<Vec3b> (y, x) = r;
      }
  return result;
}

Mat
ContrastAdjuster::adjust (const Mat & image)
{
  Mat result = Mat::zeros (image.size (), image.type ());

  for (int y = 0; y < image.rows; y++)
    for (int x = 0; x < image.cols; x++)
      {
        Vec3b src = image.at<Vec3b> (y, x);
        Vec3b r;
        r[0]                    = saturate_cast<uchar> (src[0] * contrast);
        r[1]                    = saturate_cast<uchar> (src[1] * contrast);
        r[2]                    = saturate_cast<uchar> (src[2] * contrast);
        result.at<Vec3b> (y, x) = r;
      }
  return result;
}

ImageAdjusterAlgorithm::ImageAdjusterAlgorithm (as::Config::ptr config)
  : IAbstractAlgorithm (config)
{
}

ImageAdjusterAlgorithm::~ImageAdjusterAlgorithm ()
{
  delete adjuster;
}

bool
ImageAdjusterAlgorithm::create ()
{
  type = config->get<std::string> ("adjuster.type");
  if (type == "brightness")
    {
      BrightnessAdjuster * v = new BrightnessAdjuster ();
      v->brightness = config->get<double> ("adjuster.brightness");
      adjuster      = v;
    }
  else if (type == "contrast")
    {
      ContrastAdjuster * v = new ContrastAdjuster ();
      v->contrast = config->get<double> ("adjuster.contrast");
      adjuster    = v;
    }
  else
    {
      std::cout << "create failed!" << std::endl;
      return false;
    }


  return true;
}

bool
ImageAdjusterAlgorithm::run (const Mat & image, const Mat &)
{
  Mat result = adjuster->adjust (image);

  as::Logger & l = as::Logger::instance ();

  cv::Point3d p0, p1;

  p0.x = p0.y = p0.z = 1;
  p1.x = p1.y = p1.z = 0;

  l.log_image (result, type);
  l.log_camera (p0, 0, 0, 0, "p0");
  l.log_camera (p1, 0, 0, 0, "p1");
  return true;
}

const std::string
ImageAdjusterAlgorithm::id_string (void)
{
  return std::string ("Image adjuster");
}
