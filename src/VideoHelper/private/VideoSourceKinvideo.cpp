#include <VideoSourceKinvideo.hpp>

namespace arstudio {
VideoSourceKinvideo::VideoSourceKinvideo (const std::string & file)
{
  this->source_file = file;
}

bool
VideoSourceKinvideo::init (void)
{
  current_frame = 0;
  video_capture = std::make_shared <FileCapture> (source_file);
  if (video_capture->isConnected ())
    {
      frame_count_ = video_capture->getFrameCount ();
      return true;
    }
  return false;
}

int
VideoSourceKinvideo::frame_count (void)
{
  return frame_count_;
}

bool
VideoSourceKinvideo::go_to_frame (int frame)
{
  if (frame > frame_count_ || frame < 1)
    return false;
  current_frame = frame;
  video_capture->setFrameNumber (frame);
  video_capture->readFrame ();
  LuxFrame * f = video_capture->getFrame ();
  current_image_ = f->image;
  current_dmap_  = f->depth_map;
  return true;
}

bool
VideoSourceKinvideo::next_frame (void)
{
  return go_to_frame (++current_frame);
}

const cv::Mat
VideoSourceKinvideo::image (void)
{
  return current_image_;
}

const cv::Mat
VideoSourceKinvideo::depth_map (void)
{
  return current_dmap_;
}
}
