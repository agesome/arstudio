#include <sequence.hpp>

namespace Sequence
{

void
Sequence::addItem (int type, boost::shared_ptr<Item> p)
{
	items.insert (item_mmap_t (type, p));
}

}