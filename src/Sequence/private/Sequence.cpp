#include <Sequence.hpp>

namespace Workspace
{
Sequence::Sequence (Item::type type)
{
  this->type_ = type;
}

Sequence::ptr
Sequence::make (Item::type type)
{
  return std::make_shared<Sequence> (type);
}

void
Sequence::add_item (unsigned int frame, Item::ptr item_ptr)
{
  items_.insert (std::make_pair (frame, item_ptr));
}

const Sequence::frame_map &
Sequence::items (void)
{
  return items_;
}

Item::type
Sequence::type (void)
{
  return type_;
}
}
