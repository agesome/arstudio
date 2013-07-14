#include <Scenegraph.hpp>

namespace arstudio {
Scenegraph::Scenegraph (QObject * parent)
  : QObject (parent),
  m_locked_to (NOT_LOCKED)
{
}

Scenegraph::ptr
Scenegraph::make (QObject * parent)
{
  return ptr (new Scenegraph (parent));
}

void
Scenegraph::add_sequence (Sequence * seq)
{
  m_sequences.push_back (seq);
  for (int frame : seq->items ().keys ())
    m_frames.insert (frame);

  connect (seq, &Sequence::items_changed,
           this, &Scenegraph::rebuild_frames);

  if (seq->type () == ap::Node::Bitmap)
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
Scenegraph::sequences ()
{
  return m_sequences;
}

const QSet<int>
Scenegraph::frames ()
{
  return m_frames;
}

void
Scenegraph::rebuild_frames ()
{
  m_frames.clear ();
  for (Sequence * s : m_sequences)
    for (int frame : s->items ().keys ())
      m_frames.insert (frame);
  sequences_changed ();
}

void
Scenegraph::clear ()
{
  m_sequences.clear ();
  m_frames.clear ();
  m_locked_to = NOT_LOCKED;
  sequences_changed ();
}

int
Scenegraph::locked_to () const
{
  return m_locked_to;
}
}
