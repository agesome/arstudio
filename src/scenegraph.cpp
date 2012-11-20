#include <scenegraph.hpp>

namespace Workspace
{
Scenegraph::ptr
Scenegraph::make ()
{
	return boost::make_shared<Scenegraph> ();
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
