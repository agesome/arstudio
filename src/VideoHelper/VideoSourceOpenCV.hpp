#ifndef VIDEOSOURCEOPENCV_HPP
#define VIDEOSOURCEOPENCV_HPP

#include <memory>
#include <iostream>

#include <opencv2/highgui/highgui.hpp>

#include <IVideoSource.hpp>

namespace arstudio {
class VideoSourceOpenCV : public IVideoSource
{
public:
  VideoSourceOpenCV (const std::string &);
  bool init (void);
  int frame_count (void);
  bool go_to_frame (int);
  bool next_frame (void);
  const cv::Mat image (void);
  const cv::Mat depth_map (void);
private:
  std::shared_ptr<cv::VideoCapture> video_capture;
  std::string                       source_file;
  int                               current_frame;
  int                               frame_count_;
  cv::Mat                           current_image_;
};
}
#endif
