#ifndef PROCESSING_DIALOG_H
#define PROCESSING_DIALOG_H

#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QProgressBar>
#include <QFuture>
#include <QtConcurrentRun>
#include <QRadioButton>
#include <QFileInfo>
#include <QSettings>

#include <exception>
#include <stdexcept>

#include <opencv2/highgui/highgui.hpp>
#include <boost/filesystem.hpp>

#include <AlgoPipeline.hpp>
#include <Logger.hpp>
#include <Config.hpp>
#include <ConfigEditor.hpp>

#include <filecapture.hpp>

/**
 *      This class handles processing of video files, until individual
 * frames are passed to AlgoPipeline
 */

class ProcessingDialog : public QWidget
{
  Q_OBJECT
public:
  explicit
  ProcessingDialog (QWidget * parent = nullptr);
  ~ProcessingDialog ();
private:
  void processing_thread (int, int);
  void populate_config (const std::string &);
  void ui_lock (void);
  void ui_unlock (void);
  void create_layout (void);
  void connect_signals (void);
  bool load_file (const QString &);

  QLabel       * file_name_label;
  QLabel       * frames_count_label;
  QPushButton  * select_file_button;
  QPushButton  * process_button;
  QPushButton  * stop_button;
  QSpinBox     * start_frame_spin;
  QSpinBox     * end_frame_spin;
  QProgressBar * progress_bar;
  QRadioButton * radio_whole_file;
  QRadioButton * radio_select_frames;
  QString        selected_file;
  QString        last_selected_file;
  QSettings      settings; //< save and restore path to last opened file

  cv::VideoCapture * video_capture    = nullptr; //< used for regular video
                                                 // files
  FileCapture      * kinvideo_capture = nullptr; //< used for kinvideo
                                                 // files

  Config::ptr       config_ptr;
  AlgoPipeline::ptr algo_pipeline;

  bool run_processing_thread;
signals:
  void processing_done (bool, const std::string &);
  void progress_signal (void);
  void clear_repository (void);

private slots:
  void select_file (void);
  void process_frames (void);
  void update_progress (void);
  void select_frames_changed (bool);
  void stop_clicked (void);
  void processing_cleanup (void);
};

#endif // PROCESSING_DIALOG_H
