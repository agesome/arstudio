#ifndef TIMELINEMODEL_HPP
#define TIMELINEMODEL_HPP

#include <QObject>

namespace arstudio {
/**
 *      This class implements a model for the TimeLine widget.
 */

class TimeLineModel : public QObject
{
  Q_OBJECT
public:
  TimeLineModel (int, int);

  bool set_frame (int);
  bool next_frame (void);
  void set_range (int, int);
  int min_frame (void);
  int max_frame (void);
  int current_frame (void);
private:
  int current_frame_;
  int min_frame_;
  int max_frame_;
signals:
  void new_frame (int);
};
}
#endif // TIMELINEMODEL_HPP
