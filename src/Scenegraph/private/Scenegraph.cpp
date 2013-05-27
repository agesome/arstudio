#include <Scenegraph.hpp>

namespace arstudio {
void
Scenegraph::add_sequence (Sequence * seq)
{
  sequences_.push_back (seq);
  sequences_changed ();
}

void
Scenegraph::remove_sequence (Sequence * seq)
{
  sequences_.removeOne (seq);
  sequences_changed ();
}

QQmlListProperty<arstudio::Sequence>
Scenegraph::sequences (void)
{
  return QQmlListProperty<arstudio::Sequence> (this, sequences_);
}

/**
 *      This method returns lowest frame index of all sequences contained
 *      in this instance.
 */

unsigned int
Scenegraph::min_frame (void)
{
  unsigned int frame = UINT_MAX;

  for (auto seq : sequences_)
    for (auto it : seq->items ())
      if (it.first < frame)
        frame = it.first;

  return frame;
}

/**
 *      This method returns highest frame index of all sequences contained
 *      in this instance.
 */

unsigned int
Scenegraph::max_frame (void)
{
  unsigned int frame = 0;

  for (auto seq : sequences_)
    for (auto it : seq->items ())
      if (it.first > frame)
        frame = it.first;

  return frame;
}
}
