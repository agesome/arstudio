#include <Scenegraph.hpp>

namespace Workspace
{
Scenegraph::ptr
Scenegraph::make ()
{
  return std::make_shared<Scenegraph> ();
}

void
Scenegraph::addSequence (Sequence::ptr seq)
{
  sequences.push_back (seq);
}

void
Scenegraph::removeSequence (Sequence::ptr seq)
{
  sequences.remove (seq);
}

const Scenegraph::list &
Scenegraph::getSequences (void)
{
  return sequences;
}

/**
 *      This method returns lowest frame index of all sequences contained
 *      in this instance.
 */

unsigned int
Scenegraph::getMinFrame (void)
{
  unsigned int r = UINT_MAX;

  for (auto seq : sequences)
    for (auto it : seq->getItems ())
      if (it.first < r)
        r = it.first;

  return r;
}

/**
 *      This method returns highest frame index of all sequences contained
 *      in this instance.
 */

unsigned int
Scenegraph::getMaxFrame (void)
{
  unsigned int r = 0;

  for (auto seq : sequences)
    for (auto it : seq->getItems ())
      if (it.first > r)
        r = it.first;

  return r;
}
}
