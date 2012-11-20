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
#include <QTimer>
#include <timelinemodel.hpp>
class TimeLine : public QWidget
{
	Q_OBJECT
public:
	explicit TimeLine(QWidget *parent = 0);
    TimeLine (TimeLineModel *tmlnmd, QMainWindow *MainWindow = 0);
	void setMax (int value);
    void setMin(int value);
    void updateWidget();
private:
    QGridLayout *layout = new QGridLayout (this);
    QSlider *horizontalSlider = new QSlider (this);
    QSpinBox *spinBox = new QSpinBox (this);
    QPushButton *playButton = new QPushButton (this);
    QPushButton *stopButton = new QPushButton (this);
    QToolButton *toolButton_prev = new QToolButton (this);
    QToolButton *toolButton_next = new QToolButton (this);
    QTimer *timer = new QTimer(this);
    TimeLineModel *tmlnmod;
    void connects();
    void iniGUI();
signals:
    void drawFrame ();
public slots:
    void onPrevButtonPressed ();
	void onNextButtonPressed ();
	void onSpinBoxEditFinished ();
	void onPlayPressed ();
	void onStopPressed ();
    void onModelChanged (int n);
    void onTimer();
    void onSliderChanged(int n);

};

#endif // TIMELINE_H
