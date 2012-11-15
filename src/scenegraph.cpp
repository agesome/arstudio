#include <scenegraph.hpp>

namespace Workspace
{
Scenegraph::Scenegraph (ScenegraphSelector * s)
{
	selector = s;
}

Scenegraph::ptr
Scenegraph::make (ScenegraphSelector * p)
{
	return boost::make_shared<Scenegraph> (p);
}

void
Scenegraph::addSequence (Sequence::ptr seq)
{
	Item::typemask t = selector->getSelections ();

	if (t & (1 << seq->getType ()))
		sequences.push_back (seq);
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
