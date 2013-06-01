#include <Scenegraph.hpp>

namespace arstudio {
Scenegraph::Scenegraph (QObject * parent)
  : QObject (parent)
{
}

void
Scenegraph::add_sequence (Sequence * seq)
{
  m_sequences.push_back (seq);
  for (auto i : seq->items ())
    m_frames.insert (i.first);
  sequences_changed ();
}

void
Scenegraph::remove_sequence (Sequence * seq)
{
  m_sequences.removeOne (seq);
  rebuild_frames ();
  sequences_changed ();
}

const Scenegraph::SequenceList
Scenegraph::sequences (void)
{
  return m_sequences;
}

const QSet<int>
Scenegraph::frames (void)
{
  return m_frames;
}

void
Scenegraph::rebuild_frames (void)
{
  m_frames.clear ();
  for (Sequence * s : m_sequences)
    for (auto i : s->items ())
      m_frames.insert (i.first);
}

void
Scenegraph::clear (void)
{
  m_sequences.clear ();
  m_frames.clear ();
  sequences_changed ();
}
}
