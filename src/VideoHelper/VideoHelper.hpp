#ifndef VIDEOHELPER_HPP
#define VIDEOHELPER_HPP

#include <memory>
#include <cassert>

#include <opencv2/highgui/highgui.hpp>
#include <boost/filesystem.hpp>

#include <VideoSourceOpenCV.hpp>
#include <VideoSourceKinvideo.hpp>

namespace arstudio {
namespace fs = boost::filesystem;

class VideoHelper
{
public:
  bool load_file (const std::string &);
  bool go_to_frame (int);
  bool next_frame (void);
  int frame_count (void);
  const cv::Mat image (void);
  const cv::Mat depth_map (void);
  const std::string file_basename (void);
private:
  std::shared_ptr<IVideoSource> m_video_source;
  std::string                   m_file_basename;
};
}
#endif // VIDEOHELPER_HPP
