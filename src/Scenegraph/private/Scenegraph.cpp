#include <Scenegraph.hpp>

namespace Workspace
{
Scenegraph::ptr
Scenegraph::make ()
{
  return std::make_shared<Scenegraph> ();
}

void
Scenegraph::add_sequence (Sequence::ptr seq)
{
  sequences_.push_back (seq);
}

void
Scenegraph::remove_sequence (Sequence::ptr seq)
{
  sequences_.remove (seq);
}

const Scenegraph::sequence_list &
Scenegraph::sequences (void)
{
  return sequences_;
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
