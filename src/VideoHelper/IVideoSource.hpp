#ifndef IVIDEOSOURCE_HPP
#define IVIDEOSOURCE_HPP

#include <opencv2/highgui/highgui.hpp>

namespace arstudio {
class IVideoSource
{
public:
  virtual bool init (void)               = 0;
  virtual int frame_count (void)         = 0;
  virtual bool go_to_frame (int)         = 0;
  virtual bool next_frame (void)         = 0;
  virtual const cv::Mat image (void)     = 0;
  virtual const cv::Mat depth_map (void) = 0;
  virtual
  ~IVideoSource () = default;
};
}

#endif
