#include <Sequence.hpp>

namespace arstudio {
Sequence::Sequence (ItemType type, QObject * parent)
  : QObject (parent)
{
  this->type_ = type;
}

Sequence::Sequence (QObject * parent)
  : QObject (parent)
{
}

Sequence::ptr
Sequence::make (ItemType type)
{
  return QSharedPointer<Sequence> (new Sequence (type));
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

Sequence::ItemType
Sequence::type (void)
{
  return type_;
}

arstudio::Item *
Sequence::item_for_frame (int frame)
{
  auto it = items_.find (frame);

  if (it == items_.end ())
    return nullptr;
  return it->second.data ();
}
}
