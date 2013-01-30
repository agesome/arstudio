#include <video.hpp>
Video::Video(QWidget *parent, AlgoPipeline *p) :
	QWidget (parent)
{
	file_name_label->setText ("File: ");
	frames_count_label->setText ("Frames: ");
	select_frames_label->setText ("Select frames: ");
	select_file_button->setText ("Select file");
	process_button->setText ("Process");

	layout->addWidget (file_name_label, 0, 0, 1, 3, 0);
	layout->addWidget (frames_count_label, 1, 0, 1, 3, 0);
	layout->addWidget (select_frames_label, 2, 0, 1, 1, 0);
	layout->addWidget (start_frame_spin, 2, 1, 1, 1, 0);
	layout->addWidget (end_frame_spin, 2, 2, 1, 1, 0);
	layout->addWidget (select_file_button, 3, 0, 1, 1, 0);
	layout->addWidget (process_button, 3, 1, 1, 1, 0);

	connect (select_file_button, SIGNAL (clicked ()), this, SLOT (select_file ()));
	connect (process_button, SIGNAL (clicked ()), this, SLOT (process_frames ()));

	// this->adjustSize();
	// this->resize (1, 1);
	// this->setMaximumHeight (this->height ());

	apipe = p;
}


void Video::select_file ()
{
	file_path = QFileDialog::getOpenFileName (this,
	                                          tr ("Open Video"), "~", tr ("Video Files (*.avi *.mkv *.wmv *.mp4)"));
	file_name_label->setText ("File: " + file_path);
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
}

void Video::process_frames (void)
{
	if (vcap == NULL)
		return;

	int start = start_frame_spin->value ();
	int end = end_frame_spin->value ();
	cv::Mat image, empty;

	vcap->set (CV_CAP_PROP_POS_FRAMES, start);

	for (int i = start; i < end; i++)
		{
			*vcap >> image;
			apipe->processFrame (image, empty);
		}
	done_processing ();
}
