#ifndef VIDEO_PIPELINE_HPP
#define VIDEO_PIPELINE_HPP

#include <QObject>
#include <QtConcurrent>

#include <AlgoPipeline.hpp>
#include <Config.hpp>
#include <VideoHelper.hpp>

namespace arstudio {
/**
 * Video processing pipeline
 *
 * This class is instantinied by ItemWindow QML component and is
 * responsible
 * for non-blocking processing of video files in a separate thread
 * using AlgoPipeline. It provides a convenient QML interface to both
 * VideoHelper and AlgoPipeline.
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
public:
  VideoPipeline (QObject * parent = nullptr);

  bool running (void);
  void set_running (bool);
  VideoHelper * video_helper (void);
  void set_video_helper (VideoHelper *);
  Config * config (void);
  void set_config (Config *);
  int start_frame (void);
  int end_frame (void);
  void set_start_frame (int);
  void set_end_frame (int);
  float progress (void);
private:
  void processing_thread (void);

  bool          m_run_processing;
  VideoHelper * m_video_helper;
  Config      * m_config;
  int           m_start_frame;
  int           m_end_frame;
  float         m_processing_progress;
signals:
  void running_changed (void);
  void progress_changed (void);
};
}

#endif // VIDEO_PIPELINE_HPP


//#ifndef PIPELINE_HPP
//#define PIPELINE_HPP

//#include <QObject>
//#include <QtConcurrent>

//#include <AlgoPipeline.hpp>
//#include <Config.hpp>
//#include <VideoHelper.hpp>

//using namespace arstudio;

//class Pipeline : public QObject
//{
//  Q_OBJECT
//public:
//  explicit
//  Pipeline (QObject * parent = 0);
//private:
//  void process_frames (void);
//public slots:
//  void process (void);
//signals:
//  void progress (int v, int max);
//};

//#endif // PIPELINE_HPP
