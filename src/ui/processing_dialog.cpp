#include <ui/processing_dialog.hpp>

ProcessingDialog::ProcessingDialog(QWidget *parent) :
	QWidget (parent)
{
	file_name_label->setText ("File: ");
	frames_count_label->setText ("Frames: ");
	select_file_button->setText ("Select file");
	process_button->setText ("Process");
	stop_button->setText ("Stop");

	layout->addWidget (file_name_label, 0, 0, 1, 3, 0);
	layout->addWidget (frames_count_label, 1, 0, 1, 3, 0);
	layout->addWidget (radio_whole_file, 2, 0);
	layout->addWidget (radio_select_frames, 3, 0);
	layout->addWidget (start_frame_spin, 3, 1, 1, 1, 0);
	layout->addWidget (end_frame_spin, 3, 2, 1, 1, 0);
	layout->addWidget (progress_bar, 4, 0, 1, 3, 0);
	layout->addWidget (select_file_button, 5, 0, 1, 1, 0);
	layout->addWidget (process_button, 5, 1, 1, 1, 0);
	layout->addWidget (stop_button, 5, 2, 1, 1, 0);

	radio_whole_file->setEnabled (false);
	radio_select_frames->setEnabled (false);
	start_frame_spin->setEnabled (false);
	end_frame_spin->setEnabled (false);
	process_button->setEnabled (false);
	stop_button->setEnabled (false);

	progress_bar->setFormat ("%v / %m");
	progress_bar->setValue (0);
	progress_bar->setMaximum (-1);

	connect (select_file_button, SIGNAL (clicked ()), this, SLOT (select_file ()));
	connect (process_button, SIGNAL (clicked ()), this, SLOT (process_frames ()));
	connect (this, SIGNAL (progress_signal ()), this, SLOT (update_progress ()));
	connect (radio_select_frames, SIGNAL (toggled (bool)), this,
	         SLOT (select_frames_changed (bool)));
	connect (stop_button, SIGNAL (clicked ()), this, SLOT (stop_clicked ()));

	QSize ms = layout->minimumSize ();
	// ms.setWidth (ms.width () + 200);
	this->setFixedSize (ms);
	editor->show ();
}

void ProcessingDialog::select_frames_changed (bool state)
{
	start_frame_spin->setEnabled (state);
	end_frame_spin->setEnabled (state);
}

void ProcessingDialog::stop_clicked (void)
{
	run_thread = false;
}

void ProcessingDialog::select_file ()
{
	file_path = QFileDialog::getOpenFileName (this,
	                                          tr ("Open Video"), "~", tr ("Video Files (*.avi *.mkv *.wmv *.mp4)"));
	QFileInfo f (file_path);
	file_name_label->setText ("File: " + f.baseName ());
	file_name_label->setToolTip (file_path);

	cv::VideoCapture * c = new cv::VideoCapture (file_path.toStdString ());
	int frames_count;
	if (!c->isOpened ())
		{
			frames_count = -1;
			vcap = NULL;
		}
	else
		{
			frames_count = c->get (CV_CAP_PROP_FRAME_COUNT);
			start_frame_spin->setRange (0, frames_count - 1);
			end_frame_spin->setRange (1, frames_count);
			vcap = c;
		}
	frames_count_label->setText ("Frames: " + QString::number (frames_count));

	radio_whole_file->setEnabled (true);
	radio_whole_file->setChecked (true);
	radio_select_frames->setEnabled (true);
	process_button->setEnabled (true);
}

void ProcessingDialog::process_frames (void)
{
	if (vcap == NULL)
		return;

	int start, end;
	if (radio_whole_file->isChecked ())
		{
			start = 0;
			end = vcap->get (CV_CAP_PROP_FRAME_COUNT);
		}
	else
		{
			start = start_frame_spin->value ();
			end = end_frame_spin->value ();
		}

	if (start >= end)
		return;
	progress_bar->setRange (0, end - start);
	progress_bar->setValue (1);

	radio_whole_file->setEnabled (false);
	radio_select_frames->setEnabled (false);
	start_frame_spin->setEnabled (false);
	end_frame_spin->setEnabled (false);
	process_button->setEnabled (false);
	select_file_button->setEnabled (false);
	stop_button->setEnabled (true);

	run_thread = true;
	QFuture <void> thread = QtConcurrent::run (this, &ProcessingDialog::processing_thread, start, end);
}

void ProcessingDialog::processing_thread (int start, int end)
{
	cv::Mat image, empty;

	apipe->create ();
	vcap->set (CV_CAP_PROP_POS_FRAMES, start);
	for (int i = start; i < end && run_thread; i++)
		{
			*vcap >> image;
			apipe->processFrame (image, empty);
			progress_signal ();
		}
	done_processing ();
	process_button->setEnabled (true);
	select_file_button->setEnabled (true);
	stop_button->setEnabled (false);
	radio_whole_file->setEnabled (true);
	radio_select_frames->setEnabled (true);
	start_frame_spin->setEnabled (true);
	end_frame_spin->setEnabled (true);
}

void ProcessingDialog::update_progress (void)
{
	progress_bar->setValue (progress_bar->value () + 1);
}
