#include <VideoHelper.hpp>
#include <QtDebug>

namespace arstudio {
VideoHelper::VideoHelper (QObject * parent)
  : QObject (parent),
  m_file_basename ("none"),
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

  if (!m_video_source)
    return false;

  if (!m_video_source->init ())
    return false;

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
VideoHelper::next_frame ()
{
  return m_video_source->next_frame ();
}

int
VideoHelper::frame_count ()
{
  return m_frame_count;
}

const cv::Mat
VideoHelper::image ()
{
  return m_video_source->image ();
}

const cv::Mat
VideoHelper::depth_map ()
{
  return m_video_source->depth_map ();
}

const QString
VideoHelper::file_basename ()
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
  if (!url.isValid ())
    return;
  m_status = load_file (url.path ());
  status_changed ();
}

bool
VideoHelper::status ()
{
  return m_status;
}
}
