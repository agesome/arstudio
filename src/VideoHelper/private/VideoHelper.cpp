#include <VideoHelper.hpp>

namespace arstudio {
bool
VideoHelper::load_file (const std::string & file)
{
  fs::path p (file);

  if (!fs::exists (p))
    return false;
  if (p.extension () == ".kinvideo")
    video_source = std::make_shared <VideoSourceKinvideo> (file);
  else
    video_source = std::make_shared <VideoSourceOpenCV> (file);
  assert (video_source);
  bool init = video_source->init ();
  assert (init);
  file_basename_ = fs::basename (p);
  return true;
}

bool
VideoHelper::go_to_frame (int frame)
{
  return video_source->go_to_frame (frame);
}

bool
VideoHelper::next_frame (void)
{
  return video_source->next_frame ();
}

int
VideoHelper::frame_count (void)
{
  return video_source->frame_count ();
}

const cv::Mat
VideoHelper::image (void)
{
  return video_source->image ();
}

const cv::Mat
VideoHelper::depth_map (void)
{
  return video_source->depth_map ();
}

const std::string
VideoHelper::file_basename (void)
{
  return file_basename_;
}
}
