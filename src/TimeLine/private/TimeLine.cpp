#include <TimeLine.hpp>

TimeLine::TimeLine (TimeLineModel * tmlnmd, QMainWindow * MainWindow) :
  QWidget (MainWindow)
{
  this->tmlnmod = tmlnmd;
  initGUI ();
  connects ();
  updateWidget ();
  QMetaObject::connectSlotsByName (MainWindow);
}


void
TimeLine::onSpinBoxEditFinished ()
{
  horizontalSlider->setValue (spinBox->value ());
  tmlnmod->setCurFrame (spinBox->value ());
}


void
TimeLine::onNextButtonPressed ()
{
  if (spinBox->value () < spinBox->maximum ())
    {
      spinBox->setValue (spinBox->value () + 1);
      horizontalSlider->setValue (spinBox->value ());
    }
}


void
TimeLine::onPrevButtonPressed ()
{
  if (spinBox->value () > spinBox->minimum ())
    {
      spinBox->setValue (spinBox->value () - 1);
      horizontalSlider->setValue (spinBox->value ());
    }
}


void
TimeLine::setMax (int value)
{
  spinBox->setMaximum (value);
  horizontalSlider->setMaximum (value);
}

void
TimeLine::onPlayPressed ()
{
  playButton->setEnabled (false);
  stopButton->setEnabled (true);
  toolButton_next->setEnabled (false);
  toolButton_prev->setEnabled (false);
  // timer on
  timer->start (100);
}


void
TimeLine::onStopPressed ()
{
  // timer off
  timer->stop ();
  playButton->setEnabled (true);
  stopButton->setEnabled (false);
  toolButton_next->setEnabled (true);
  toolButton_prev->setEnabled (true);
}



void
TimeLine::onModelChanged (int n)
{
  horizontalSlider->setValue (n);
  spinBox->setValue (n);
}


void
TimeLine::onTimer ()
{
  if (!tmlnmod->incCurFrame ())
    onStopPressed ();
}


void
TimeLine::setMin (int value)
{
  spinBox->setMinimum (value);
  horizontalSlider->setMinimum (value);
}


void
TimeLine::updateWidget ()
{
  setMax (tmlnmod->getMax ());
  setMin (tmlnmod->getMin ());
}


void
TimeLine::connects ()
{
  connect (toolButton_prev, SIGNAL (clicked ()), this,
           SLOT (onPrevButtonPressed ()));
  connect (toolButton_next, SIGNAL (clicked ()), this,
           SLOT (onNextButtonPressed ()));
  connect (playButton, SIGNAL (clicked ()), this, SLOT (onPlayPressed ()));
  connect (stopButton, SIGNAL (clicked ()), this, SLOT (onStopPressed ()));
  connect (spinBox, SIGNAL (editingFinished ()), this,
           SLOT (onSpinBoxEditFinished ()));
  connect (tmlnmod, SIGNAL (newFrame (int)), this, SLOT (onModelChanged (int)));
  connect (timer, SIGNAL (timeout ()), this, SLOT (onTimer ()));
  connect (horizontalSlider, SIGNAL (valueChanged (int)), this,
           SLOT (onSliderChanged (int)));
}


void
TimeLine::onSliderChanged (int n)
{
  spinBox->setValue (n);
  tmlnmod->setCurFrame (n);
}

void
TimeLine::initGUI ()
{
  layout->addWidget (horizontalSlider, 0, 0, 1, 6);
  horizontalSlider->setOrientation (Qt::Horizontal);
  horizontalSlider->setTickInterval (1);
  horizontalSlider->setPageStep (1);
  horizontalSlider->setTickPosition (QSlider::TicksLeft);
  layout->addWidget (toolButton_prev, 1, 0, 0);
  toolButton_prev->setText ("<");
  layout->addWidget (toolButton_next, 1, 1, 0);
  toolButton_next->setText (">");
  layout->addWidget (playButton, 1, 2, 0);
  playButton->setText ("Play");
  layout->addWidget (stopButton, 1, 3, 0);
  stopButton->setText ("Stop");
  stopButton->setEnabled (false);
  layout->addWidget (spinBox, 1, 5, 0);
}
