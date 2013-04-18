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
 * This singleton class recieves data from algorithms, converts it to
 * a suitable format and stores it in the Repository.
 *
 * All log_ methods have an overload which specifies a sane default name
 *for
 * the items logged.
 */

class Logger
{
public:
  static Logger & instance (void);
  static void set_repository (const Workspace::Repository::ptr);

  void log_image (const cv::Mat &, const std::string &);
  void log_image (const cv::Mat &);
  void log_point (cv::Point3d point, const std::string &);
  void log_point (cv::Point3d point);
  void log_camera (cv::Point3d, double, double, double, const std::string &);
  void log_camera (cv::Point3d, double, double, double);
  void advance_frame (void);
  void reset_frame_counter (void);
private:
  Logger ();
  Logger (const Logger &)             = delete;
  Logger & operator= (const Logger &) = delete;

  static Logger                     instance_;
  static Workspace::Repository::ptr repo; //< currently used instance
  unsigned int                      current_frame = 1; //< log sequences
                                                       // for this frame
};

#endif // LOGGER_H
