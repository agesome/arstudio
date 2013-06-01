#ifndef SCENEGRAPH_AGGREGATOR_HPP
#define SCENEGRAPH_AGGREGATOR_HPP

#include <QQmlEngine>
#include <Scenegraph.hpp>

namespace arstudio {
class ScenegraphAggregator : public QObject
{
  Q_OBJECT
  Q_PROPERTY (int max_frame READ max_frame NOTIFY limits_changed)
  Q_PROPERTY (int min_frame READ min_frame NOTIFY limits_changed)
public:
  static ScenegraphAggregator *
  instance (void)
  {
    return m_instance;
  }

  static QObject *
  qml_instance (QQmlEngine *, QJSEngine *)
  {
    return m_instance;
  }

  Q_INVOKABLE void add_scenegraph (Scenegraph * scenegraph);
  Q_INVOKABLE void remove_scenegraph (Scenegraph * scenegraph);

  int min_frame (void);
  int max_frame (void);
  Q_INVOKABLE bool valid_frame (int frame);
  Q_INVOKABLE void signal_frame (int frame);
private:
  ScenegraphAggregator (void);

  void rebuild_frames (void);
  void recalculate_limits (void);

  static ScenegraphAggregator * m_instance;
  QList<Scenegraph *>           m_scenegraph_list;
  QSet<int>                     m_frames;
  int                           m_min_frame;
  int                           m_max_frame;
signals:
  void limits_changed (void);
  void change_frame (int);
public slots:
  void repository_clearing (void);
};
}
#endif // SCENEGRAPH_AGGREGATOR_HPP
