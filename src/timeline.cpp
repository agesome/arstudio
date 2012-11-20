#include <timeline.hpp>
#include <qapplication.h>
#include <QDebug>
TimeLine::TimeLine(TimeLineModel *tmlnmd, QMainWindow *MainWindow) :
	QWidget (MainWindow)
{
<<<<<<< HEAD
    this->tmlnmod = tmlnmd;
    iniGUI();
    connects();
    updateWidget();
=======
	this->setObjectName (QString::fromUtf8 ("TimeLine"));
	loop_flag = false;
	this->setMinimumHeight (90);
	this->setMaximumHeight (90);
	layout = new QGridLayout (this);
	horizontalSlider = new QSlider (this);
	layout->addWidget (horizontalSlider, 0, 0, 0, 6, 0);
	horizontalSlider->setObjectName (QString::fromUtf8 ("horizontalSlider"));
	horizontalSlider->setOrientation (Qt::Horizontal);
	horizontalSlider->setMinimum (1);
	horizontalSlider->setTickInterval (1);
	horizontalSlider->setPageStep (1);
	horizontalSlider->setTickPosition (QSlider::TicksLeft);
	spinBox = new QSpinBox (this);
	layout->addWidget (spinBox, 1, 5, 0);
	spinBox->setObjectName (QString::fromUtf8 ("spinBox"));
	spinBox->setMinimum (1);
	playButton = new QPushButton (this);
	layout->addWidget (playButton, 1, 2, 0);
	playButton->setObjectName (QString::fromUtf8 ("playButton"));
	playButton->setText ("Play");
	stopButton = new QPushButton (this);
	layout->addWidget (stopButton, 1, 3, 0);
	stopButton->setObjectName (QString::fromUtf8 ("stopButton"));
	stopButton->setText ("Stop");
	toolButton_prev = new QToolButton (this);
	layout->addWidget (toolButton_prev, 1, 0, 0);
	toolButton_prev->setObjectName (QString::fromUtf8 ("toolButton_prev"));
	toolButton_prev->setText ("<");
	toolButton_next = new QToolButton (this);
	layout->addWidget (toolButton_next, 1, 1, 0);
	toolButton_next->setObjectName (QString::fromUtf8 ("toolButton_next"));
	toolButton_next->setText (">");
	setMax (max);
	connect (horizontalSlider, SIGNAL (valueChanged (int)), this, SLOT (onSliderValueChanged (int)));
	connect (toolButton_prev, SIGNAL (clicked ()), this, SLOT (onPrevButtonPressed ()));
	connect (toolButton_next, SIGNAL (clicked ()), this, SLOT (onNextButtonPressed ()));
	connect (playButton, SIGNAL (clicked ()), this, SLOT (onPlayPressed ()));
	connect (stopButton, SIGNAL (clicked ()), this, SLOT (onStopPressed ()));
	connect (spinBox, SIGNAL (editingFinished ()), this, SLOT (onSpinBoxEditFinished ()));


	// retranslateUi(MainWindow);
	QMetaObject::connectSlotsByName (MainWindow);
}
>>>>>>> 86866e2bc19b9b471850248534400c8101f4ded7

    QMetaObject::connectSlotsByName (MainWindow);
}


void TimeLine::onSpinBoxEditFinished ()
{
	horizontalSlider->setValue (spinBox->value ());
    tmlnmod->setCurFrame(spinBox->value ());
}


void TimeLine::onNextButtonPressed ()
{
	if (spinBox->value () < spinBox->maximum ())
		{
			spinBox->setValue (spinBox->value () + 1);
			horizontalSlider->setValue (spinBox->value ());
		}
}


void TimeLine::onPrevButtonPressed ()
{
	if (spinBox->value () > spinBox->minimum ())
		{
			spinBox->setValue (spinBox->value () - 1);
			horizontalSlider->setValue (spinBox->value ());
		}
}


void TimeLine::setMax (int value)
{
	spinBox->setMaximum (value);
	horizontalSlider->setMaximum (value);
}
/*void TimeLine::onDrawed ()
{
	if (loop_flag)
		{
			if (horizontalSlider->value () == horizontalSlider->maximum ())
				{
					onStopPressed ();
					return;
				}
			qApp->processEvents ();
			horizontalSlider->setValue (horizontalSlider->value () + 1);  // it's emit the slider~>valueChanged signal
		}
}*/

void TimeLine::onPlayPressed ()
{
	qDebug () << "play";
    playButton->setEnabled(false);
    stopButton->setEnabled(true);
	toolButton_next->setEnabled (false);
	toolButton_prev->setEnabled (false);
    //timer on
    timer->start(100);
}


void TimeLine::onStopPressed ()
{
    //timer off
    timer->stop();
    playButton->setEnabled(true);
    stopButton->setEnabled(false);
    toolButton_next->setEnabled (true);
    toolButton_prev->setEnabled (true);

}



void TimeLine::onModelChanged(int n)
{
    horizontalSlider->setValue (n);
    spinBox->setValue (n);

}


void TimeLine::onTimer()
{
    qDebug()<<"hello";
    if(!tmlnmod->incCurFrame())
        onStopPressed();

}


void TimeLine::setMin(int value)
{
    spinBox->setMinimum (value);
    horizontalSlider->setMinimum (value);
}


void TimeLine::updateWidget()
{
    setMax (tmlnmod->getMax());
    setMin (tmlnmod->getMin());

}


void TimeLine::connects()
{
    connect (toolButton_prev, SIGNAL (clicked ()), this, SLOT (onPrevButtonPressed ()));
    connect (toolButton_next, SIGNAL (clicked ()), this, SLOT (onNextButtonPressed ()));
    connect (playButton, SIGNAL (clicked ()), this, SLOT (onPlayPressed ()));
    connect (stopButton, SIGNAL (clicked ()), this, SLOT (onStopPressed ()));
    connect (spinBox, SIGNAL (editingFinished ()), this, SLOT (onSpinBoxEditFinished ()));
    connect (tmlnmod, SIGNAL(newFrame (int)), this, SLOT(onModelChanged(int)));
    connect (timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    connect (horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(onSliderChanged(int)));
}


void TimeLine::onSliderChanged(int n)
{
    spinBox->setValue(n);
    tmlnmod->setCurFrame(n);
}


void TimeLine::iniGUI()
{
    this->setMinimumHeight (90);
    this->setMaximumHeight (90);
    layout->addWidget (horizontalSlider, 0, 0, 0, 6, 0);
    horizontalSlider->setOrientation (Qt::Horizontal);
    horizontalSlider->setTickInterval (1);
    horizontalSlider->setPageStep (1);
    horizontalSlider->setTickPosition (QSlider::TicksLeft);
    layout->addWidget (spinBox, 1, 5, 0);
    layout->addWidget (playButton, 1, 2, 0);
    playButton->setText ("Play");
    layout->addWidget (stopButton, 1, 3, 0);
    stopButton->setText ("Stop");
    stopButton->setEnabled(false);
    layout->addWidget (toolButton_prev, 1, 0, 0);
    toolButton_prev->setText ("<");
    layout->addWidget (toolButton_next, 1, 1, 0);
    toolButton_next->setText (">");

}
