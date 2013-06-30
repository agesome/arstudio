#ifndef VIDEO_PIPELINE_HPP
#define VIDEO_PIPELINE_HPP

#include <QObject>
#include <QtConcurrent>
#include <QImage>
#include <QMutex>
#include <QMutexLocker>

#include <AlgoPipeline.hpp>
#include <Config.hpp>
#include <VideoHelper.hpp>

namespace arstudio {
/**
 * @brief Video processing pipeline
 *
 * This class is instantinied by ItemWindow QML component and is responsible for
 * non-blocking processing of video files in a separate thread using
 * AlgoPipeline. It provides a convenient QML interface to both VideoHelper
 * and AlgoPipeline.
 */
class VideoPipeline : public QObject
{
  Q_OBJECT
  Q_PROPERTY (bool running READ running WRITE set_running NOTIFY running_changed)
  Q_PROPERTY (arstudio::VideoHelper * video_helper READ video_helper
              WRITE set_video_helper)
  Q_PROPERTY (arstudio::Config * config READ config WRITE set_config)
  Q_PROPERTY (int start_frame READ start_frame WRITE set_start_frame)
  Q_PROPERTY (int end_frame READ end_frame WRITE set_end_frame)
  Q_PROPERTY (float progress READ progress NOTIFY progress_changed)
  Q_PROPERTY (QImage current_image READ current_image
              NOTIFY current_image_changed)
public:
  VideoPipeline (QObject * parent = nullptr);

  /**
   * @brief Processing status
   * @return true if currently processing, false otherwise
   */
  bool running ();
  /**
   * @brief Control processing
   * @param value start processing if true, stop otherwise
   */
  void set_running (bool value);

  /**
   * @param value frame from which to begin processing
   */
  void set_start_frame (int value);
  /**
   * @param value frame at which to end processing
   */
  void set_end_frame (int value);
  /**
   * @return current processing progress, in range [0,1]
   */
  float progress ();

  /**
   * @return a QImage of the frame currently being processed
   */
  QImage current_image ();


  VideoHelper * video_helper ();
  void set_video_helper (VideoHelper *);
  Config * config ();
  void set_config (Config *);
  int start_frame ();
  int end_frame ();

private:
  void processing_thread ();

  bool          m_run_processing;
  VideoHelper * m_video_helper;
  Config      * m_config;
  int           m_start_frame;
  int           m_end_frame;
  float         m_processing_progress;
  QMutex        m_image_access_mutex;
  QImage        m_current_image;
signals:
  void running_changed ();
  void progress_changed ();
  void current_image_changed ();
};
}

#endif // VIDEO_PIPELINE_HPP
