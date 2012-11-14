#include <scenegraph.hpp>

namespace Workspace
{

Scenegraph::Scenegraph (ScenegraphSelector::ptr s)
{
	selector = s;
}

Scenegraph::ptr
Scenegraph::make (ScenegraphSelector::ptr p)
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

}
