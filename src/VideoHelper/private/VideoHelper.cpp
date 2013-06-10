#include <VideoHelper.hpp>

namespace arstudio {
VideoHelper::VideoHelper (QObject * parent)
  : QObject (parent),
  m_file_basename ("invalid"),
  m_status (false),
  m_frame_count (0)
{
  frame_count_changed ();
  basename_changed ();
  status_changed ();
}

bool
VideoHelper::load_file (const QString & file)
{
  QFileInfo f (file);

  if (!f.exists ())
    return false;
  if (f.suffix () == "kinvideo")
    m_video_source = QSharedPointer <VideoSourceKinvideo> (
      new VideoSourceKinvideo (file));
  else
    m_video_source = QSharedPointer <VideoSourceOpenCV> (
      new VideoSourceOpenCV (file));
  Q_ASSERT (m_video_source);

  bool init = m_video_source->init ();
  Q_ASSERT (init);

  m_file_basename = f.baseName ();
  m_source_file   = file;
  m_frame_count   = m_video_source->frame_count ();

  basename_changed ();
  frame_count_changed ();

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
  return m_frame_count;
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

const QString
VideoHelper::file_basename (void)
{
  return m_file_basename;
}

const QUrl
VideoHelper::source_file ()
{
  return m_source_file;
}

void
VideoHelper::set_source (const QUrl & url)
{
  m_status = load_file (url.path ());
  status_changed ();
}

bool
VideoHelper::status (void)
{
  return m_status;
}
}
