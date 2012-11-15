#include <timeline.hpp>
#include <qapplication.h>
TimeLine::TimeLine(int max, QMainWindow *MainWindow) :
	QWidget(MainWindow)
{
	this->setObjectName(QString::fromUtf8("TimeLine"));
    loop_flag = false;
    this->setMinimumHeight(90);
    this->setMaximumHeight(90);
	layout = new QGridLayout(this);
	horizontalSlider = new QSlider(this);
	layout->addWidget(horizontalSlider, 0, 0, 0, 6, 0);
	horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
	horizontalSlider->setOrientation(Qt::Horizontal);
	horizontalSlider->setMinimum(1);
	horizontalSlider->setTickInterval(1);
	horizontalSlider->setPageStep(1);
	horizontalSlider->setTickPosition(QSlider::TicksLeft);
	spinBox = new QSpinBox(this);
	layout->addWidget(spinBox, 1, 5, 0);
	spinBox->setObjectName(QString::fromUtf8("spinBox"));
	spinBox->setMinimum(1);
	playButton = new QPushButton(this);
	layout->addWidget(playButton, 1, 2, 0);
	playButton->setObjectName(QString::fromUtf8("playButton"));
	playButton->setText("Play");
	stopButton = new QPushButton(this);
	layout->addWidget(stopButton, 1, 3, 0);
	stopButton->setObjectName(QString::fromUtf8("stopButton"));
	stopButton->setText("Stop");
	toolButton_prev = new QToolButton(this);
	layout->addWidget(toolButton_prev, 1, 0, 0);
	toolButton_prev->setObjectName(QString::fromUtf8("toolButton_prev"));
	toolButton_prev->setText("<");
	toolButton_next = new QToolButton(this);
	layout->addWidget(toolButton_next, 1, 1, 0);
	toolButton_next->setObjectName(QString::fromUtf8("toolButton_next"));
	toolButton_next->setText(">");
    setMax(max);
	connect(horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged(int)));
	connect(toolButton_prev, SIGNAL(clicked()), this, SLOT(onPrevButtonPressed()));
	connect(toolButton_next, SIGNAL(clicked()), this, SLOT(onNextButtonPressed()));
	connect(playButton, SIGNAL(clicked()), this, SLOT(onPlayPressed()));
	connect(stopButton, SIGNAL(clicked()), this, SLOT(onStopPressed()));
	connect(spinBox, SIGNAL(editingFinished()), this, SLOT(onSpinBoxEditFinished()));


	//retranslateUi(MainWindow);
    QMetaObject::connectSlotsByName(MainWindow);
}

void TimeLine::onSliderValueChanged(int value)
{
	spinBox->setValue(value);
	onOkPressed();
}


void TimeLine::onSpinBoxEditFinished()
{
	horizontalSlider->setValue(spinBox->value());
	onOkPressed();
}


void TimeLine::onNextButtonPressed()
{
	if(spinBox->value() < spinBox->maximum())
		{
			spinBox->setValue(spinBox->value()+1);
			horizontalSlider->setValue(spinBox->value());

		}

}


void TimeLine::onPrevButtonPressed()
{
	if(spinBox->value() > spinBox->minimum())
		{
			spinBox->setValue(spinBox->value()-1);
			horizontalSlider->setValue(spinBox->value());
		}

}


void TimeLine::setMax(int value)
{
	spinBox->setMaximum(value);
	horizontalSlider->setMaximum(value);

}
void TimeLine::onDrawed()
{
	if(loop_flag)
        {
            if(horizontalSlider->value() == horizontalSlider->maximum())
                {
                    onStopPressed();
                    return;
                }
            qApp->processEvents();
            horizontalSlider->setValue(horizontalSlider->value()+1);//it's emit the slider~>valueChanged signal

        }

}

void TimeLine::onPlayPressed()
{
	qDebug()<<"play";
	toolButton_next->setEnabled(false);
	toolButton_prev->setEnabled(false);
	loop_flag = true;
	emit nextFrame(horizontalSlider->value());
}


void TimeLine::onStopPressed()
{
    if(loop_flag)
		{
			loop_flag = false;
            toolButton_next->setEnabled(true);
			toolButton_prev->setEnabled(true);
		}
}


void TimeLine::onOkPressed()
{
	emit nextFrame(horizontalSlider->value());

}
