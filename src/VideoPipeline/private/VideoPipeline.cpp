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
  connect (qApp, &QApplication::aboutToQuit, [this]() {
             m_run_processing = false;
           });
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
    {
      float to_process = m_end_frame - m_start_frame + 1;
      // this way, all algorithms can optionally find out how many frames there are
      // this should not be a parameter of process_frame(), because some algorithms
      // may not want this
      m_config->set ("internal.total_frames", to_process);
      QtConcurrent::run (this, &VideoPipeline::processing_thread, to_process);
    }
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

QImage
VideoPipeline::current_image ()
{
  QMutexLocker lock (&m_image_access_mutex);

  return m_current_image;
}

void
VideoPipeline::processing_thread (float frames_to_process)
{
  AlgoPipeline::ptr ap = AlgoPipeline::make ();
  cv::Mat           rgb;
  QImage            i;

  ap->create_all (m_config);
  m_video_helper->go_to_frame (m_start_frame);

  int frames_processed = 0;

  do
    {
      cv::cvtColor (m_video_helper->image (), rgb, CV_BGR2RGB);
      i = QImage (rgb.data, rgb.cols, rgb.rows, rgb.step,
                  QImage::Format_RGB888);
      m_image_access_mutex.lock ();
      // have to get a copy, i relies on data in rgb which will be gone
      // when this function exits
      m_current_image = i.copy ();
      m_image_access_mutex.unlock ();
      current_image_changed ();

      ap->process_frame (m_video_helper->image (),
                         m_video_helper->depth_map (),
                         frames_processed + 1);

      frames_processed++;
      m_processing_progress = frames_processed / frames_to_process;

      /*
       * m_run_processing may be manually set to false on
       * QApplication::aboutToQuit; that indicates that user has closed the
       * main window, QML objects have been destroyed and calling
       * progress_changed will cause a segfault; so we don't
       */
      if (m_run_processing)
        progress_changed ();

      if (frames_processed == frames_to_process)
        break;
    } while (m_run_processing && m_video_helper->next_frame ());

  // check if aboutToQuit() has fired; see comment above
  if (m_run_processing)
    set_running (false);
}
}
