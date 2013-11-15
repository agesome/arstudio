#ifndef SCENEGRAPH_AGGREGATOR_HPP
#define SCENEGRAPH_AGGREGATOR_HPP

#include <QQmlEngine>
#include <Scenegraph.hpp>

namespace arstudio {
/**
 * @brief The Scenegraph aggregator
 *
 * This class is responsible for aggregating frame sets for all existing
 * instances of Scenegraph and providing information for TimeLine, as well as
 * providing centralized control over all Scenegraph instances via signal_frame,
 * thus allowing simultaneous playback for all ItemWindow instances.
 * This class is a singleton.
 */

class ScenegraphAggregator : public QObject
{
  Q_OBJECT
  Q_PROPERTY (int max_frame READ max_frame NOTIFY limits_changed)
  Q_PROPERTY (int min_frame READ min_frame NOTIFY limits_changed)
public:
  ~ScenegraphAggregator ();

  static ScenegraphAggregator *
  instance ()
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

  int min_frame ();
  int max_frame ();
  Q_INVOKABLE void signal_frame (int frame);
private:
  ScenegraphAggregator ();

  void rebuild_frames ();
  void recalculate_limits ();

  static ScenegraphAggregator * m_instance;
  QList<Scenegraph *>           m_scenegraph_list;
  QSet<int>                     m_frames;
  int                           m_min_frame;
  int                           m_max_frame;
signals:
  void limits_changed ();
  void change_frame (int);
public slots:
  void repository_clearing ();
};
}
#endif // SCENEGRAPH_AGGREGATOR_HPP
