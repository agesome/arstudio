#include <Sequence.hpp>

namespace arstudio {
Sequence::Sequence (ItemType type, QObject * parent)
  : QObject (parent)
{
  m_type = type;
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
  m_items.insert (std::make_pair (frame, item_ptr));
}

const Sequence::frame_map &
Sequence::items (void)
{
  return m_items;
}

Sequence::ItemType
Sequence::type (void)
{
  return m_type;
}

arstudio::Item *
Sequence::item_for_frame (int frame)
{
  auto it = m_items.find (frame);

  if (it == m_items.end ())
    return nullptr;
  return it->second.data ();
}
}
