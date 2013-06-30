#ifndef ALGO_INTERFACE_HPP
#define ALGO_INTERFACE_HPP

#include <string>

#include <opencv2/highgui/highgui.hpp>

#include <Config.hpp>

namespace as = arstudio;

/**
 * @brief The IAbstractAlgorithm class is an interface for data processing
 * algotihms
 */

class IAbstractAlgorithm
{
public:
  /**
   * @brief IAbstractAlgorithm
   * @param config is stored internally and is available to child classes
   */
  IAbstractAlgorithm (as::Config * config)
    : config (config)
  {
  }

  virtual
  ~IAbstractAlgorithm ()
  {
  }

  /**
   * @brief initialize the algorithm; do not throw exceptions!
   * @return true on success, false otherwise
   */
  virtual bool create () = 0;

  /**
   * @brief process a single image + depth map frame
   * @param image RGB888 image
   * @param dmap depth map
   * @return true on success, false otherwise
   */
  virtual bool run (const cv::Mat & image, const cv::Mat & dmap) = 0;

  /**
   * @return a string identifying this algorithm to the user
   */
  virtual const std::string id_string () = 0;

protected:
  /**
   * @brief Config instance for this algorithm
   */
  as::Config * config;
};

#endif // ALGO_INTERFACE_HPP
