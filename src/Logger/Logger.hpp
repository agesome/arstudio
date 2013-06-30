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

namespace arstudio {
/**
 * @brief Logger is responsible for storing processing results from algorithms
 *
 * This class converts data recieved from algorithms to a format suitable for
 * Repository and stores it there.
 */

class Logger
{
public:
  static Logger & instance ();
  static void set_repository (const Repository::ptr);

  void log_image (const cv::Mat &, const std::string &);
  void log_image (const cv::Mat &);
  void log_point (const cv::Point3d & point, const std::string & name);
  void log_point (const cv::Point3d & point);
  void log_camera (const cv::Point3d & pos, const cv::Point3d & r,
                   const std::string & name);
  void log_camera (const cv::Point3d & pos, const cv::Point3d & r);
  void advance_frame ();
  void reset_frame_counter ();
private:
  Logger ()                           = default;
  Logger (const Logger &)             = delete;
  Logger & operator= (const Logger &) = delete;

  static Logger          m_instance;
  static Repository::ptr m_repository;
  /**
   * @brief All log_ functions store data for this frame
   */
  unsigned int m_current_frame = 1;
};
}

#endif // LOGGER_H
