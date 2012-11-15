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

Sequence::ptr
Sequence::make (Item::type t)
{
	return boost::make_shared<Sequence> (t);
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
