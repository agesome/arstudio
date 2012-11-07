#ifndef TIMELINE_H
#define TIMELINE_H
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>
#include <QWidget>
#include <QDebug>
#include <QGridLayout>
class TimeLine : public QWidget
{
	Q_OBJECT
public:
	explicit TimeLine(QWidget *parent = 0);
	TimeLine (int max, QMainWindow *MainWindow=0);
	void setMax(int value);
private:
	QGridLayout *layout;
	QSlider *horizontalSlider;
	QSpinBox *spinBox;
	QPushButton *playButton;
	QPushButton *stopButton;
	QToolButton *toolButton_prev;
	QToolButton *toolButton_next;
	bool loop_flag;
signals:
	void nextFrame(int);
public slots:
	void onSliderValueChanged(int value);
	void onPrevButtonPressed();
	void onNextButtonPressed();
	void onSpinBoxEditFinished();
	void onPlayPressed();
	void onStopPressed();
	void onOkPressed();
	void onDrawed();
};

#endif // TIMELINE_H
