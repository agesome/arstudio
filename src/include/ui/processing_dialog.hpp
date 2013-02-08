#ifndef PROCESSING_DIALOG_H
#define PROCESSING_DIALOG_H

#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QGridLayout>
#include <QFileDialog>
#include <QProgressBar>
#include <QFuture>
#include <QtConcurrentRun>

#include <opencv2/highgui/highgui.hpp>
#include <algo_pipeline.hpp>

class ProcessingDialog : public QWidget
{
	Q_OBJECT
public:
	explicit ProcessingDialog (QWidget *parent = 0);
private:
	QLabel *file_name_label = new QLabel (this);
	QLabel *frames_count_label = new QLabel (this);
	QLabel *select_frames_label = new QLabel (this);
	QPushButton *select_file_button = new QPushButton (this);
	QPushButton *process_button = new QPushButton (this);
	QSpinBox *start_frame_spin = new QSpinBox (this);
	QSpinBox *end_frame_spin = new QSpinBox (this);
	QProgressBar *progress_bar = new QProgressBar (this);
	QGridLayout *layout = new QGridLayout (this);
	QString file_path;

	cv::VideoCapture *vcap = NULL;

	void processing_thread (int, int);
signals:
	void done_processing ();
	void progress_signal ();

public slots:
	void select_file ();
	void process_frames ();
	void update_progress ();
};
#endif // PROCESSING_DIALOG_H
