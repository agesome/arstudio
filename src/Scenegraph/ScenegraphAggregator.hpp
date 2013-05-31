#ifndef SCENEGRAPH_AGGREGATOR_HPP
#define SCENEGRAPH_AGGREGATOR_HPP

#include <Scenegraph.hpp>
#include <QDebug>

namespace arstudio {
class ScenegraphAggregator : public QObject
{
  Q_OBJECT
  Q_PROPERTY (int max_frame READ max_frame NOTIFY max_frame_changed)
  Q_PROPERTY (int min_frame READ min_frame NOTIFY min_frame_changed)
public:
  typedef QSharedPointer<ScenegraphAggregator> ptr;

  ScenegraphAggregator (QObject * parent = nullptr);

  static ptr
  make (QObject * parent = nullptr)
  {
    return ptr (new ScenegraphAggregator (parent));
  }

  Q_INVOKABLE void add_scenegraph (Scenegraph * scenegraph);
  Q_INVOKABLE void remove_scenegraph (Scenegraph * scenegraph);

  int min_frame (void);
  int max_frame (void);
  Q_INVOKABLE bool valid_frame (int frame);
  Q_INVOKABLE void signal_frame (int frame);
private:
  void rebuild_frames (void);
  void recalculate_limits (void);

  QList<Scenegraph *> m_scenegraph_list;
  QSet<int>           m_frames;
  int                 m_min_frame;
  int                 m_max_frame;
signals:
  void min_frame_changed (void);
  void max_frame_changed (void);
  void change_frame (int);
};
}
#endif // SCENEGRAPH_AGGREGATOR_HPP
