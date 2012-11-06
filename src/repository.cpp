#include <repository.hpp>

namespace Workspace
{

void
Repository::addSequence (int id, seq_ptr p)
{
	sequences.insert (seq_map_t (id, p));
}

unsigned char
Repository::getItemTypes (void)
{
	unsigned char r = 0;

	for (auto it: sequences)
		{
			r |= (1 << it.second->type);
		}
	return r;
}

}