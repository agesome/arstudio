#include <sequence.hpp>

namespace Workspace
{

Sequence::Sequence (item_type t)
{
	type = t;
}

void
Sequence::addItem (int nframe, item_ptr p)
{
	items.insert (item_map_t (nframe, p));
}

}