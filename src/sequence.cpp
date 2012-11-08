#include <sequence.hpp>

namespace Workspace
{

Sequence::Sequence (item_type t)
{
	type = t;
}

void
Sequence::addItem (int nframe, Item::ptr p)
{
	items.insert (Item::pair (nframe, p));
}

const Sequence::map &
Sequence::getItems (void)
{
	return items;
}

item_type
Sequence::getType (void)
{
	return type;
}

}
