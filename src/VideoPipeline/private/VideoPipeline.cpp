#include <VideoPipeline.hpp>

namespace arstudio {
VideoPipeline::VideoPipeline (QObject * parent)
  : QObject (parent),
  m_run_processing (false),
  m_video_helper (nullptr),
  m_config (nullptr),
  m_start_frame (1),
  m_end_frame (1),
  m_processing_progress (0)
{
}

bool
VideoPipeline::running ()
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
VideoPipeline::video_helper ()
{
  return m_video_helper;
}

void
VideoPipeline::set_video_helper (VideoHelper * vh)
{
  m_video_helper = vh;
}

Config *
VideoPipeline::config ()
{
  return m_config;
}

void
VideoPipeline::set_config (Config * ptr)
{
  m_config = ptr;
}

int
VideoPipeline::start_frame ()
{
  return m_start_frame;
}

int
VideoPipeline::end_frame ()
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
VideoPipeline::progress ()
{
  return m_processing_progress;
}

QImage VideoPipeline::current_image()
{
  QMutexLocker lock (&m_image_access_mutex);
  return m_current_image;
}

void
VideoPipeline::processing_thread ()
{
  AlgoPipeline::ptr ap = AlgoPipeline::make ();
  cv::Mat rgb;


  ap->create_all (m_config);
  m_video_helper->go_to_frame (m_start_frame);

  const float to_process       = m_end_frame - m_start_frame + 1;
  int         frames_processed = 0;

  do
    {
      cv::cvtColor(m_video_helper->image(), rgb, CV_BGR2RGB);
      m_image_access_mutex.lock();
      m_current_image = QImage (rgb.data, rgb.cols, rgb.rows, rgb.step,
                                QImage::Format_RGB888);
      m_image_access_mutex.unlock();
      current_image_changed();

      ap->process_frame (m_video_helper->image (),
                         m_video_helper->depth_map ());

      frames_processed++;
      m_processing_progress = frames_processed / to_process;
      progress_changed ();

      if (frames_processed == to_process)
        break;
    } while (m_run_processing && m_video_helper->next_frame ());
  set_running (false);
}
}
