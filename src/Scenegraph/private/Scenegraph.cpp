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
  sequences_changed ();
}

void
Scenegraph::remove_sequence (Sequence * seq)
{
  m_sequences.removeOne (seq);
  sequences_changed ();
}

const Scenegraph::SequenceList
Scenegraph::sequences (void)
{
  return m_sequences;
}

/**
 *      This method returns lowest frame index of all sequences contained
 *      in this instance.
 */

int
Scenegraph::min_frame (void)
{
  unsigned int frame = UINT_MAX;

  for (auto seq : m_sequences)
    for (auto it : seq->items ())
      if (it.first < frame)
        frame = it.first;

  return frame;
}

/**
 *      This method returns highest frame index of all sequences contained
 *      in this instance.
 */

int
Scenegraph::max_frame (void)
{
  unsigned int frame = 0;

  for (auto seq : m_sequences)
    for (auto it : seq->items ())
      if (it.first > frame)
        frame = it.first;

  return frame;
}
}
