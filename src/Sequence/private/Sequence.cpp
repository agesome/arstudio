#include <Sequence.hpp>

namespace arstudio {
Sequence::Sequence (ap::Node::Type type, QObject * parent)
  : QObject (parent),
  m_type (type)
{
}

Sequence::Sequence (QObject * parent)
  : QObject (parent),
  m_type (ap::Node::Invalid)
{
}

Sequence::ptr
Sequence::make (ap::Node::Type type)
{
  return QSharedPointer<Sequence> (new Sequence (type));
}

void
Sequence::add_item (int frame, const Item::ptr item_ptr)
{
  m_items.insert (frame, item_ptr);
  items_changed ();
}

const Sequence::FrameMap &
Sequence::items ()
{
  return m_items;
}

ap::Node::Type
Sequence::type ()
{
  return m_type;
}

const Item::ptr
Sequence::item_for_frame (int frame)
{
  return m_items.value (frame);
}
}
