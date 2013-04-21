#include <ProcessingDialog.hpp>

namespace arstudio {
ProcessingDialog::ProcessingDialog (QWidget * parent) :
  QWidget (parent)
{
  this->config_ptr = Config::make ();
  create_layout ();
  connect_signals ();

  // defined in CMakeLists.txt
  populate_config ("@CONFIG_DIRECTORY@");

  last_selected_file =
    settings.value ("ProcessingDialog/lastSelectedFile", "~").toString ();
  if (last_selected_file != "~")
    load_file (last_selected_file);
}

void
ProcessingDialog::processing_cleanup (void)
{
  ui_unlock ();
  this->hide ();
}

ProcessingDialog::~ProcessingDialog ()
{
  settings.setValue ("ProcessingDialog/lastSelectedFile", last_selected_file);
}

void
ProcessingDialog::create_layout (void)
{
  QGridLayout  * layout        = new QGridLayout (this);
  ConfigEditor * config_editor = new ConfigEditor (config_ptr);

  file_name_label     = new QLabel ("File: ", this);
  frames_count_label  = new QLabel ("Frames: ", this);
  select_file_button  = new QPushButton ("Select file", this);
  process_button      = new QPushButton ("Process", this);
  stop_button         = new QPushButton ("Stop", this);
  start_frame_spin    = new QSpinBox (this);
  end_frame_spin      = new QSpinBox (this);
  progress_bar        = new QProgressBar (this);
  radio_whole_file    = new QRadioButton ("Process whole file", this);
  radio_select_frames = new QRadioButton ("Select frames", this);

  layout->addWidget (file_name_label, 0, 0, 1, 3);
  layout->addWidget (frames_count_label, 1, 0, 1, 3);
  layout->addWidget (radio_whole_file, 2, 0);
  layout->addWidget (radio_select_frames, 3, 0);
  layout->addWidget (start_frame_spin, 3, 1, 1, 1);
  layout->addWidget (end_frame_spin, 3, 2, 1, 1);
  layout->addWidget (progress_bar, 4, 0, 1, 3);
  layout->addWidget (select_file_button, 5, 0, 1, 1);
  layout->addWidget (process_button, 5, 1, 1, 1);
  layout->addWidget (stop_button, 5, 2, 1, 1);
  layout->addWidget (config_editor, 0, 3, 6, 8);

  ui_lock ();
  select_file_button->setEnabled (true);
  stop_button->setEnabled (false);

  progress_bar->setFormat ("%v / %m");
  progress_bar->setValue (0);
  progress_bar->setMaximum (-1);
}

void
ProcessingDialog::connect_signals (void)
{
  connect (select_file_button, SIGNAL (clicked ()), this,
           SLOT (select_file ()));
  connect (process_button, SIGNAL (clicked ()), this, SLOT (process_frames ()));
  connect (this, SIGNAL (progress_signal ()), this, SLOT (update_progress ()));
  connect (radio_select_frames, SIGNAL (toggled (bool)), this,
           SLOT (select_frames_changed (bool)));
  connect (stop_button, SIGNAL (clicked ()), this, SLOT (stop_clicked ()));

  connect (this, SIGNAL (processing_done (bool, const std::string &)), this,
           SLOT (processing_cleanup (void)));
}

/**
 *      This method imports settings from every directory at path, by
 * convention
 *      -- from a file named "settings.xml".
 *
 *      \param path the directory to scan for sub-directories
 */

void
ProcessingDialog::populate_config (const std::string & path)
{
  boost::filesystem::directory_iterator dir (path), end;

  for (; dir != end; dir++)
    {
      boost::filesystem::path p = dir->path ();
      if (boost::filesystem::is_directory (p))
        config_ptr->import_xml (p.string () + "/settings.xml");
    }
}

void
ProcessingDialog::select_frames_changed (bool state)
{
  start_frame_spin->setEnabled (state);
  end_frame_spin->setEnabled (state);
}

void
ProcessingDialog::stop_clicked (void)
{
  run_processing_thread = false;
}

void
ProcessingDialog::select_file (void)
{
  selected_file =
    QFileDialog::getOpenFileName (this,
                                  "Open Video",
                                  last_selected_file,
                                  "Video Files (*.avi *.mkv *.wmv *.mp4 *.kinvideo)");
  if (selected_file.isNull ())
    return;
  last_selected_file = selected_file;
  load_file (selected_file);
}

/**
 *      This method loads a video file either with OpenCV VideoCapture, or
 * with
 *      FileCapture for kinvide.
 *      \param path path to the file
 */

bool
ProcessingDialog::load_file (const QString & path)
{
  QFileInfo file_info (path);

  if (file_info.isFile () == false)
    return false;
  unsigned int frames_count;

  file_name_label->setText ("File: " + file_info.baseName ());
  file_name_label->setToolTip (selected_file);
  radio_whole_file->setEnabled (true);
  radio_whole_file->setChecked (true);
  process_button->setEnabled (true);

  if (kinvideo_capture)
    {
      delete kinvideo_capture;
      kinvideo_capture = nullptr;
    }
  else if (video_capture)
    {
      delete video_capture;
      video_capture = nullptr;
    }

  if (file_info.suffix () == "kinvideo")
    {
      kinvideo_capture = new FileCapture (path.toStdString ());
      frames_count     = kinvideo_capture->getFrameCount ();
    }
  else
    {
      cv::VideoCapture * video_capture =
        new cv::VideoCapture (path.toStdString ());

      if (!video_capture->isOpened ())
        {
          frames_count  = -1;
          video_capture = NULL;
        }
      else
        {
          frames_count = video_capture->get (CV_CAP_PROP_FRAME_COUNT);
          start_frame_spin->setRange (0, frames_count - 1);
          end_frame_spin->setRange (1, frames_count);
          this->video_capture = video_capture;
          radio_select_frames->setEnabled (true);
        }
    }
  frames_count_label->setText ("Frames: " + QString::number (frames_count));
  return true;
}

/**
 *      This method determines the range of frames to be processed and
 * starts the processing thread.
 */

void
ProcessingDialog::process_frames (void)
{
  if (!video_capture && !kinvideo_capture)
    return;

  int start, end;
  if (radio_whole_file->isChecked ())
    {
      start = 0;
      if (video_capture)
        end = video_capture->get (CV_CAP_PROP_FRAME_COUNT);
      else
        end = kinvideo_capture->getFrameCount ();
    }
  else
    {
      start = start_frame_spin->value ();
      end   = end_frame_spin->value ();
    }

  if (start >= end)
    return;
  progress_bar->setRange (0, end - start);
  progress_bar->setValue (1);

  ui_lock ();
  clear_repository ();

  run_processing_thread = true;

  algo_pipeline = AlgoPipeline::make (config_ptr);
  try
    {
      algo_pipeline->create_all ();
    }
  catch (const std::runtime_error & error)
    {
      processing_done (false, error.what ());
      return;
    }

  QFuture <void> thread =
    QtConcurrent::run (this, &ProcessingDialog::processing_thread, start, end);
}

void
ProcessingDialog::processing_thread (int start, int end)
{
  cv::Mat image, empty;

  if (video_capture)
    video_capture->set (CV_CAP_PROP_POS_FRAMES, start);

  for (int i = start; i < end && run_processing_thread; i++)
    {
      try
        {
          if (video_capture)
            {
              *video_capture >> image;
              algo_pipeline->process_frame (image, empty);
            }
          else
            {
              kinvideo_capture->readFrame ();
              LuxFrame * f = kinvideo_capture->getFrame ();
              algo_pipeline->process_frame (f->image, f->depth_map);
            }
        }
      catch (const std::out_of_range & error)
        {
          processing_done (false, error.what ());
          return;
        }
      progress_signal ();
    }
  processing_done (true, std::string ());
}

void
ProcessingDialog::update_progress (void)
{
  progress_bar->setValue (progress_bar->value () + 1);
}

void
ProcessingDialog::ui_lock (void)
{
  radio_whole_file->setEnabled (false);
  radio_select_frames->setEnabled (false);
  start_frame_spin->setEnabled (false);
  end_frame_spin->setEnabled (false);
  process_button->setEnabled (false);
  select_file_button->setEnabled (false);
  stop_button->setEnabled (true);
}

void
ProcessingDialog::ui_unlock (void)
{
  process_button->setEnabled (true);
  select_file_button->setEnabled (true);
  stop_button->setEnabled (false);
  radio_whole_file->setEnabled (true);
  radio_select_frames->setEnabled (true);
  start_frame_spin->setEnabled (true);
  end_frame_spin->setEnabled (true);
}
}
