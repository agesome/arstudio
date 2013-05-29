#include <VideoHelper.hpp>

namespace arstudio {
bool
VideoHelper::load_file (const std::string & file)
{
  fs::path p (file);

  if (!fs::exists (p))
    return false;
  if (p.extension () == ".kinvideo")
    m_video_source = std::make_shared <VideoSourceKinvideo> (file);
  else
    m_video_source = std::make_shared <VideoSourceOpenCV> (file);
  assert (m_video_source);
  bool init = m_video_source->init ();
  assert (init);
  m_file_basename = fs::basename (p);
  return true;
}

bool
VideoHelper::go_to_frame (int frame)
{
  return m_video_source->go_to_frame (frame);
}

bool
VideoHelper::next_frame (void)
{
  return m_video_source->next_frame ();
}

int
VideoHelper::frame_count (void)
{
  return m_video_source->frame_count ();
}

const cv::Mat
VideoHelper::image (void)
{
  return m_video_source->image ();
}

const cv::Mat
VideoHelper::depth_map (void)
{
  return m_video_source->depth_map ();
}

const std::string
VideoHelper::file_basename (void)
{
  return m_file_basename;
}
}
