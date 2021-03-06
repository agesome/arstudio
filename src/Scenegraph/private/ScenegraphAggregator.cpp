#include <ScenegraphAggregator.hpp>

namespace arstudio {
// QQmlengine owns singletons and will delete them even if CppOwnership is set
ScenegraphAggregator * ScenegraphAggregator::m_instance =
  new ScenegraphAggregator ();

ScenegraphAggregator::ScenegraphAggregator ()
  : QObject (nullptr),
  m_min_frame (1),
  m_max_frame (1)
{
}

int
ScenegraphAggregator::min_frame ()
{
  return m_min_frame;
}

int
ScenegraphAggregator::max_frame ()
{
  return m_max_frame;
}

void
ScenegraphAggregator::rebuild_frames ()
{
  m_frames.clear ();
  for (Scenegraph * s : m_scenegraph_list)
    m_frames += s->frames ();
  recalculate_limits ();
}

void
ScenegraphAggregator::recalculate_limits ()
{
  if (m_frames.empty ())
    {
      m_max_frame = m_min_frame = 1;
      limits_changed ();
      return;
    }

  QList<int> v = m_frames.values ();
  qSort (v);

  m_max_frame = v.last ();
  m_min_frame = v.first ();
  limits_changed ();
}

ScenegraphAggregator::~ScenegraphAggregator ()
{
  m_instance = nullptr;
}

void
ScenegraphAggregator::add_scenegraph (Scenegraph * scenegraph)
{
  m_scenegraph_list.append (scenegraph);
  connect (scenegraph, &Scenegraph::sequences_changed,
           this, &ScenegraphAggregator::rebuild_frames);
}

void
ScenegraphAggregator::remove_scenegraph (Scenegraph * scenegraph)
{
  disconnect (scenegraph, &Scenegraph::sequences_changed,
              this, &ScenegraphAggregator::rebuild_frames);
  m_scenegraph_list.removeOne (scenegraph);
  rebuild_frames ();
}

void
ScenegraphAggregator::signal_frame (int frame)
{
  if (m_frames.contains (frame))
    change_frame (frame);
}

void
ScenegraphAggregator::repository_clearing ()
{
  for (Scenegraph * s : m_scenegraph_list)
    s->clear ();
  rebuild_frames ();
}
}
