#ifndef TIMELINE_H
#define TIMELINE_H

#include <QVariant>
#include <QAction>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QWidget>
#include <QGridLayout>
#include <QTimer>
#include <QToolButton>
#include <QMainWindow>

#include <TimeLineModel.hpp>

/**
 *      This widget handles data playback.
 */

class TimeLine : public QWidget
{
	Q_OBJECT
public:
	explicit
	TimeLine (QWidget * parent = 0);
	TimeLine (TimeLineModel * tmlnmd, QMainWindow * MainWindow = 0);
	void setMax (int value);
	void setMin (int value);
	void updateWidget ();
private:
	QGridLayout   * layout           = new QGridLayout (this);
	QSlider       * horizontalSlider = new QSlider (this);
	QSpinBox      * spinBox          = new QSpinBox (this);
	QPushButton   * playButton       = new QPushButton (this);
	QPushButton   * stopButton       = new QPushButton (this);
	QToolButton   * toolButton_prev  = new QToolButton (this);
	QToolButton   * toolButton_next  = new QToolButton (this);
	QTimer        * timer            = new QTimer (this);
	TimeLineModel * tmlnmod;
	void connects ();
	void initGUI ();
signals:
	void drawFrame ();
public slots:
	void onPrevButtonPressed ();
	void onNextButtonPressed ();
	void onSpinBoxEditFinished ();
	void onPlayPressed ();
	void onStopPressed ();
	void onModelChanged (int n);
	void onTimer ();
	void onSliderChanged (int n);
};

#endif // TIMELINE_H
