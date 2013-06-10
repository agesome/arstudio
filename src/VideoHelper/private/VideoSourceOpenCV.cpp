#include <VideoSourceOpenCV.hpp>

namespace arstudio {
VideoSourceOpenCV::VideoSourceOpenCV (const QString & file)
  : m_source_file (file)
{
}

bool
VideoSourceOpenCV::init (void)
{
  current_frame   = 0;
  m_video_capture = QSharedPointer <cv::VideoCapture> (
    new cv::VideoCapture (m_source_file.toStdString ()));
  if (m_video_capture->isOpened ())
    {
      m_frame_count = m_video_capture->get (CV_CAP_PROP_FRAME_COUNT);
      return true;
    }
  return false;
}

int
VideoSourceOpenCV::frame_count (void)
{
  return m_frame_count;
}

bool
VideoSourceOpenCV::go_to_frame (int frame)
{
  --frame; // we use 1-based frame indexing, opencv uses 0-based
  if (frame > m_frame_count - 1 || frame < 0)
    return false;
  current_frame = frame;
  m_video_capture->set (CV_CAP_PROP_POS_FRAMES, frame);
  *m_video_capture >> m_current_image;
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
  return m_current_image;
}

const cv::Mat
VideoSourceOpenCV::depth_map (void)
{
  return cv::Mat ();
}
}
