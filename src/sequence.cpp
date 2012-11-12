#include <sequence.hpp>

namespace Workspace
{

const std::string Item::typeNames[] =
{
	std::string ("Camera"),
	std::string ("Point cloud"),
	std::string ("FP Vector")
};

Sequence::Sequence (Item::type t)
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

Item::type
Sequence::getType (void)
{
	return type;
}

}
