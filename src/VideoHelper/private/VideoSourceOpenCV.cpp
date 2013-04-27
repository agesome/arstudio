#include <VideoSourceOpenCV.hpp>

namespace arstudio {
VideoSourceOpenCV::VideoSourceOpenCV (const std::string & file)
{
  this->source_file = file;
}

bool
VideoSourceOpenCV::init (void)
{
  current_frame = 0;
  video_capture = std::make_shared <cv::VideoCapture> (source_file);
  if (video_capture->isOpened ())
    {
      frame_count_ = video_capture->get (CV_CAP_PROP_FRAME_COUNT);
      return true;
    }
  return false;
}

int
VideoSourceOpenCV::frame_count (void)
{
  return frame_count_;
}

bool
VideoSourceOpenCV::go_to_frame (int frame)
{
  --frame; // we use 1-based frame indexing, opencv uses 0-based
  if (frame > frame_count_ - 1 || frame < 0)
    return false;
  current_frame = frame;
  video_capture->set (CV_CAP_PROP_POS_FRAMES, frame);
  *video_capture >> current_image_;
  return true;
}

bool
VideoSourceOpenCV::next_frame (void)
{
  return go_to_frame (++current_frame);
}

const cv::Mat
VideoSourceOpenCV::image (void)
{
  return current_image_;
}

const cv::Mat
VideoSourceOpenCV::depth_map (void)
{
  return cv::Mat ();
}
}
