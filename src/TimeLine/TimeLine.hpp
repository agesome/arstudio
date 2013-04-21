#ifndef TIMELINE_H
#define TIMELINE_H

#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QGridLayout>
#include <QTimer>

#include <TimeLineModel.hpp>

namespace arstudio {
/**
 *      This widget handles data playback.
 */

class TimeLine : public QWidget
{
  Q_OBJECT
public:
  TimeLine (TimeLineModel *, QWidget * main_window = nullptr);
  void set_range (int, int);
  void update (void);
private:
  void connect_signals (void);
  void init_gui (void);

  QSlider       * slider;
  QSpinBox      * spinbox;
  QPushButton   * play_button;
  QPushButton   * stop_button;
  QPushButton   * prev_button;
  QPushButton   * next_button;
  QTimer        * timer;
  TimeLineModel * timeline_model;
private slots:
  void prev_button_pressed (void);
  void next_button_pressed (void);
  void spinbox_edited (void);
  void play_pressed (void);
  void stop_pressed (void);
  void model_changed (int n);
  void timer_fired (void);
  void slider_changed (int n);
};
}

#endif // TIMELINE_H
