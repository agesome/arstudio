#include <repository.hpp>

namespace Workspace
{

void
Repository::addSequence (int id, Sequence::ptr p)
{
	sequences.insert (Sequence::pair (id, p));
}

unsigned char
Repository::getItemTypes (void)
{
	unsigned char r = 0;

	for (auto it: sequences)
		{
			r |= (1 << it.second->getType ());
		}
	return r;
}

const Repository::map &
Repository::getSequences (void)
{
	return sequences;
}

}
