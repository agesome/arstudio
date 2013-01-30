#ifndef VIDEO_H
#define VIDEO_H

#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QGridLayout>
#include <QFileDialog>
#include <QProgressBar>
#include <opencv2/highgui/highgui.hpp>
#include <algo_pipeline.hpp>
#include <QDebug>

class Video : public QWidget
{
	Q_OBJECT
public:
	explicit Video(QWidget *parent = 0, AlgoPipeline * p = NULL);
private:
	QLabel *file_name_label = new QLabel (this);
	QLabel *frames_count_label = new QLabel (this);
	QLabel *select_frames_label = new QLabel (this);
	QPushButton *select_file_button = new QPushButton (this);
	QPushButton *process_button = new QPushButton (this);
	QSpinBox *start_frame_spin = new QSpinBox (this);
	QSpinBox *end_frame_spin = new QSpinBox (this);
    QProgressBar *progress_bar = new QProgressBar(this);
	QGridLayout *layout = new QGridLayout (this);
	QString file_path;

	cv::VideoCapture *vcap = NULL;
	AlgoPipeline * apipe = NULL;
signals:
	void done_processing ();

public slots:
	void select_file ();
	void process_frames ();
};
#endif // VIDEO_H
