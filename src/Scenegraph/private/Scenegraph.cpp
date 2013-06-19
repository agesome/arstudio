#include <Scenegraph.hpp>

namespace arstudio {
Scenegraph::Scenegraph (QObject * parent)
  : QObject (parent),
  m_locked_to (NOT_LOCKED)
{
}

void
Scenegraph::add_sequence (Sequence * seq)
{
  m_sequences.push_back (seq);
  for (auto i : seq->items ())
    m_frames.insert (i.first);

  connect (seq, &Sequence::items_changed,
           this, &Scenegraph::rebuild_frames);

  if (seq->type () == Sequence::BITMAP)
    m_locked_to = BITMAP;
  else
    m_locked_to = NORMAL;

  locked_to_changed ();
  sequences_changed ();
}

void
Scenegraph::remove_sequence (Sequence * seq)
{
  if (m_sequences.count () == 1)
    {
      m_locked_to = NOT_LOCKED;
      locked_to_changed ();
    }

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
  sequences_changed ();
}

void
Scenegraph::clear (void)
{
  m_sequences.clear ();
  m_frames.clear ();
  sequences_changed ();
}

int
Scenegraph::locked_to (void) const
{
  return m_locked_to;
}
}
