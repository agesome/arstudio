#include <VideoSourceOpenCV.hpp>

namespace arstudio {
VideoSourceOpenCV::VideoSourceOpenCV (const QString & file)
  : m_source_file (file)
{
}

bool
VideoSourceOpenCV::init ()
{
  current_frame   = 0;
  m_video_capture = QSharedPointer <cv::VideoCapture> (new cv::VideoCapture ());

#ifdef HAVE_LIBMAGIC
  magic_t cookie = magic_open (MAGIC_MIME_TYPE);
  magic_load (cookie, NULL);
  QString d (magic_file (cookie, m_source_file.toLocal8Bit ().data ()));
  if (!d.contains ("video"))
    {
      qWarning () << "libmagic suggests that" << m_source_file
                  << "is not a video file; will not proceed";
      return false;
    }
  magic_close (cookie);
#endif

  if (!m_video_capture->open (m_source_file.toStdString ()))
    return false;

  // try reading from opened file, see if we fail
  if (!m_video_capture->grab ())
    {
      qWarning () << "VideoCaptureOpenCV failed to grab frame; "
                     "bad file format?";
      return false;
    }

  m_video_capture->set (CV_CAP_PROP_POS_FRAMES, 0);
  m_frame_count = m_video_capture->get (CV_CAP_PROP_FRAME_COUNT);
  return true;
}

int
VideoSourceOpenCV::frame_count ()
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
VideoSourceOpenCV::next_frame ()
{
  return go_to_frame (++current_frame);
}

const cv::Mat
VideoSourceOpenCV::image ()
{
  return m_current_image;
}

const cv::Mat
VideoSourceOpenCV::depth_map ()
{
  return cv::Mat ();
}
}
