#include <Sequence.hpp>

namespace arstudio {
Item::~Item ()
{
}

Sequence::Sequence (ItemType type, QObject * parent)
  : QObject (parent),
  m_type (type)
{
}

Sequence::Sequence (QObject * parent)
  : QObject (parent),
  m_type (Sequence::Invalid)
{
}

Sequence::ptr
Sequence::make (Sequence::ItemType type)
{
  return QSharedPointer<Sequence> (new Sequence (type));
}

void
Sequence::add_item (int frame, const Item::ptr item_ptr)
{
  m_framemap_mutex.lock ();
  m_items.insert (frame, item_ptr);
  m_framemap_mutex.unlock ();
  items_changed ();
}

const Sequence::FrameMap &
Sequence::items () const
{
  return m_items;
}

Sequence::ItemType
Sequence::type () const
{
  return m_type;
}

const Item::ptr
Sequence::item_for_frame (int frame) const
{
  Item::ptr rv;

  m_framemap_mutex.lock ();
  rv = m_items.value (frame);
  m_framemap_mutex.unlock ();
  return rv;
}
}
