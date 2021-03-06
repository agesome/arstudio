#include <VideoSourceKinvideo.hpp>

namespace arstudio {
VideoSourceKinvideo::VideoSourceKinvideo (const QString & file)
  : m_source_file (file)
{
}

bool
VideoSourceKinvideo::init ()
{
  m_current_frame = 0;
  m_video_capture = QSharedPointer <FileCapture> (
    new FileCapture (m_source_file.toStdString ()));
  if (m_video_capture->isConnected ())
    {
      m_frame_count = m_video_capture->getFrameCount ();
      return true;
    }
  return false;
}

int
VideoSourceKinvideo::frame_count ()
{
  return m_frame_count;
}

bool
VideoSourceKinvideo::go_to_frame (int frame)
{
  if (frame > m_frame_count || frame < 1)
    return false;
  m_current_frame = frame;
  m_video_capture->setFrameNumber (frame);
  m_video_capture->readFrame ();
  LuxFrame * f = m_video_capture->getFrame ();
  m_current_image = f->image;
  m_current_dmap  = f->depth_map;
  return true;
}

bool
VideoSourceKinvideo::next_frame ()
{
  return go_to_frame (++m_current_frame);
}

const cv::Mat
VideoSourceKinvideo::image ()
{
  return m_current_image;
}

const cv::Mat
VideoSourceKinvideo::depth_map ()
{
  return m_current_dmap;
}
}
