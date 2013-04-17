#ifndef LOGGER_H
#define LOGGER_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/nonfree/features2d.hpp>

#include <Repository.hpp>
#include <PointCloud.hpp>
#include <Bitmap.hpp>
#include <Point3D.hpp>
#include <Camera.hpp>

/**
 *      This singleton class recieves data from algorithms, converts it to
 * a suitable format
 *      and stores it in the Repository
 */

class Logger
{
public:
  static Logger & getInstance (void);
  static void setRepository (const Workspace::Repository::ptr);

  void addImage (cv::Mat &, std::string);
  void logPoint (cv::Point3d point);
  void logCamera (cv::Point3d, double, double, double);
  void advanceFrame (void);
  void resetFrameCounter (void);
private:
  Logger ();
  Logger (const Logger &)             = delete;
  Logger & operator= (const Logger &) = delete;

  static Workspace::Repository::ptr repo;       // < currently used
                                                // instance of
                                                // Repository
  unsigned int current_frame = 1;                            // < all data
                                                             // being
                                                             // logged is
                                                             // associated
                                                             // with
                                                             // this frame
                                                             // number
};

#endif // LOGGER_H
