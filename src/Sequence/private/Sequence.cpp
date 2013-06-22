#include <Sequence.hpp>

namespace arstudio {
Sequence::Sequence (ItemType type, QObject * parent)
  : QObject (parent),
  m_type (type)
{
}

Sequence::Sequence (QObject * parent)
  : QObject (parent),
  m_type (INVALID)
{
}

Sequence::ptr
Sequence::make (ItemType type)
{
  return QSharedPointer<Sequence> (new Sequence (type));
}

void
Sequence::add_item (int frame, const Item::ptr item_ptr)
{
  m_items.insert (std::make_pair (frame, item_ptr));
  items_changed ();
}

const Sequence::frame_map &
Sequence::items ()
{
  return m_items;
}

Sequence::ItemType
Sequence::type ()
{
  return m_type;
}

const Item::ptr
Sequence::item_for_frame (int frame)
{
  auto it = m_items.find (frame);

  if (it == m_items.end ())
    return Item::ptr ();
  return it->second;
}
}
