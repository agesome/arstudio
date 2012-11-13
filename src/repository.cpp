#include <repository.hpp>

namespace Workspace
{

void
Repository::addSequence (int id, Sequence::ptr p)
{
	sequences.insert (Sequence::pair (id, p));
}

void
Repository::addItem (int seq_id, Item::type it, int nfr, Item::ptr ip)
{
	Sequence::ptr p;

for (auto i: sequences)
		{
			if (i.first == seq_id)
				p = i.second;
		}
	if (!p) // p not assigned
		{
			p = boost::make_shared<Sequence> (it);
			sequences.insert (Sequence::pair (seq_id, p));
		}
	p->addItem (nfr, ip);
}

Item::typemask
Repository::getItemTypes (void)
{
	Item::typemask r = 0;

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
