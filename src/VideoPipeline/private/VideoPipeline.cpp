#include <VideoPipeline.hpp>
#include <QtDebug>

namespace arstudio {
VideoPipeline::VideoPipeline (QObject * parent)
  : QObject (parent),
  m_run_processing (false),
  m_video_helper (nullptr),
  m_start_frame (1),
  m_end_frame (1),
  m_processing_progress (0)
{
}

bool
VideoPipeline::running (void)
{
  return m_run_processing;
}

void
VideoPipeline::set_running (bool v)
{
  m_run_processing = v;
  running_changed ();
  if (v)
    QtConcurrent::run (this, &VideoPipeline::processing_thread);
}

VideoHelper *
VideoPipeline::video_helper (void)
{
  return m_video_helper;
}

void
VideoPipeline::set_video_helper (VideoHelper * vh)
{
  m_video_helper = vh;
}

int
VideoPipeline::start_frame (void)
{
  return m_start_frame;
}

int
VideoPipeline::end_frame (void)
{
  return m_end_frame;
}

void
VideoPipeline::set_start_frame (int v)
{
  m_start_frame = v;
}

void
VideoPipeline::set_end_frame (int v)
{
  m_end_frame = v;
}

float
VideoPipeline::progress (void)
{
  return m_processing_progress;
}

void
VideoPipeline::processing_thread (void)
{
  Config::ptr c = Config::make ();

  c->import_xml (
    "/home/age/Sources/arstudio/src/algorithms/slam/settings.xml");
  AlgoPipeline::ptr ap = AlgoPipeline::make (c);

  ap->create_all ();
  m_video_helper->go_to_frame (m_start_frame);

  const float to_process    = m_end_frame - m_start_frame;
  int         current_frame = 1;
  qDebug () << m_start_frame << " " << m_end_frame << " " << to_process;
  do
    {
      m_processing_progress = current_frame / to_process;
      current_frame++;
      progress_changed ();

      ap->process_frame (m_video_helper->image (),
                         m_video_helper->depth_map ());

      if (current_frame == m_end_frame)
        break;
    } while (m_run_processing && m_video_helper->next_frame ());
  set_running (false);
}
}
