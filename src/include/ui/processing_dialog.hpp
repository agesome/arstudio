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

#include <opencv2/highgui/highgui.hpp>
#include <boost/filesystem.hpp>

#include <algo_pipeline.hpp>
#include <logger.hpp>
#include <config.hpp>

#include <ui/config_editor.hpp>

#include <filecapture.hpp>

class ProcessingDialog : public QWidget
{
	Q_OBJECT
public:
	explicit ProcessingDialog (QWidget *parent = nullptr);
	~ProcessingDialog ();
private:
	QLabel *file_name_label = new QLabel (this);
	QLabel *frames_count_label = new QLabel (this);
	QPushButton *select_file_button = new QPushButton (this);
	QPushButton *process_button = new QPushButton (this);
	QPushButton *stop_button = new QPushButton (this);
	QSpinBox *start_frame_spin = new QSpinBox (this);
	QSpinBox *end_frame_spin = new QSpinBox (this);
	QProgressBar *progress_bar = new QProgressBar (this);
	QGridLayout *layout = new QGridLayout (this);
	QRadioButton *radio_whole_file = new QRadioButton ("Process whole file", this);
	QRadioButton *radio_select_frames = new QRadioButton ("Select frames", this);
	QString selectedFile;
	QString lastSelectedFile;

	QSettings settings;

	cv::VideoCapture *vcap = nullptr;
	FileCapture *kincap = nullptr;

	Config::ptr config = Config::make ();
	// has to be created before AlgoPipeline!
	ConfigEditor * editor = new ConfigEditor (config);
	AlgoPipeline::ptr apipe;

	bool run_thread;

	void processing_thread (int, int);
	void populateConfig (std::string);
	void lockUI ();
	void unlockUI ();
	void createLayout ();
signals:
	void done_processing (bool, std::string);
	void progress_signal ();

public slots:
	void select_file ();
	void process_frames ();
	void update_progress ();
	void select_frames_changed (bool);
	void stop_clicked ();
};
#endif // PROCESSING_DIALOG_H
