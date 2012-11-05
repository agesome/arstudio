#include <repository.hpp>

namespace Workspace
{

void
Repository::addSequence (int id, seq_ptr p)
{
	sequences.insert (seq_map_t (id, p));
}

}