#include <TimeLine.hpp>

TimeLine::TimeLine (TimeLineModel * timeline_model, QWidget * main_window) :
  QWidget (main_window)
{
  this->timeline_model = timeline_model;
  init_gui ();
  connect_signals ();
  update ();
  QMetaObject::connectSlotsByName (main_window);
}

void
TimeLine::spinbox_edited (void)
{
  slider->setValue (spinbox->value ());
  timeline_model->set_frame (spinbox->value ());
}

void
TimeLine::next_button_pressed (void)
{
  if (spinbox->value () < spinbox->maximum ())
    {
      spinbox->setValue (spinbox->value () + 1);
      slider->setValue (spinbox->value ());
    }
}

void
TimeLine::prev_button_pressed (void)
{
  if (spinbox->value () > spinbox->minimum ())
    {
      spinbox->setValue (spinbox->value () - 1);
      slider->setValue (spinbox->value ());
    }
}

void
TimeLine::set_range (int min, int max)
{
  spinbox->setMinimum (min);
  spinbox->setMaximum (max);
  slider->setMinimum (min);
  slider->setMaximum (max);

  spinbox->setFixedWidth (spinbox->sizeHint ().width ());
}

void
TimeLine::play_pressed (void)
{
  play_button->setEnabled (false);
  stop_button->setEnabled (true);
  next_button->setEnabled (false);
  prev_button->setEnabled (false);
  // timer on
  timer->start (100);
}

void
TimeLine::stop_pressed (void)
{
  // timer off
  timer->stop ();
  play_button->setEnabled (true);
  stop_button->setEnabled (false);
  next_button->setEnabled (true);
  prev_button->setEnabled (true);
}

void
TimeLine::model_changed (int frame)
{
  slider->setValue (frame);
  spinbox->setValue (frame);
}

void
TimeLine::timer_fired (void)
{
  if (!timeline_model->next_frame ())
    stop_pressed ();
}

void
TimeLine::update (void)
{
  set_range (timeline_model->min_frame (), timeline_model->max_frame ());
}

void
TimeLine::connect_signals (void)
{
  connect (prev_button, SIGNAL (clicked ()), this,
           SLOT (prev_button_pressed ()));
  connect (next_button, SIGNAL (clicked ()), this,
           SLOT (next_button_pressed ()));
  connect (play_button, SIGNAL (clicked ()), this, SLOT (play_pressed ()));
  connect (stop_button, SIGNAL (clicked ()), this, SLOT (stop_pressed ()));
  connect (spinbox, SIGNAL (editingFinished ()), this,
           SLOT (spinbox_edited ()));
  connect (timeline_model, SIGNAL (new_frame (int)), this,
           SLOT (model_changed (int)));
  connect (timer, SIGNAL (timeout ()), this, SLOT (timer_fired ()));
  connect (slider, SIGNAL (valueChanged (int)), this,
           SLOT (slider_changed (int)));
}

void
TimeLine::slider_changed (int frame)
{
  spinbox->setValue (frame);
  timeline_model->set_frame (frame);
}

void
TimeLine::init_gui (void)
{
  QGridLayout * layout = new QGridLayout (this);

  slider      = new QSlider (this);
  spinbox     = new QSpinBox (this);
  play_button = new QPushButton (this);
  stop_button = new QPushButton (this);
  prev_button = new QPushButton (this);
  next_button = new QPushButton (this);
  timer       = new QTimer (this);

  layout->addWidget (slider, 0, 0, 1, 6);
  slider->setOrientation (Qt::Horizontal);
  slider->setTickInterval (1);
  slider->setPageStep (1);
  slider->setTickPosition (QSlider::TicksLeft);

  QIcon icon;

  layout->addWidget (prev_button, 1, 0, 0);
  icon = QIcon::fromTheme ("go-previous");
  if (icon.isNull ())
    prev_button->setText ("<");
  else
    prev_button->setIcon (icon);
  prev_button->setFixedWidth (prev_button->sizeHint ().width ());

  layout->addWidget (next_button, 1, 1, 0);
  icon = QIcon::fromTheme ("go-next");
  if (icon.isNull ())
    next_button->setText (">");
  else
    next_button->setIcon (QIcon::fromTheme ("go-next"));
  next_button->setFixedWidth (next_button->sizeHint ().width ());

  layout->addWidget (play_button, 1, 2, 0);
  play_button->setText ("Play");
  layout->addWidget (stop_button, 1, 3, 0);
  stop_button->setText ("Stop");
  stop_button->setEnabled (false);

  layout->addWidget (spinbox, 1, 5, 0);
  spinbox->setFixedWidth (spinbox->sizeHint ().width ());
}
