#include <Repository.hpp>

namespace arstudio {
Repository::Repository (QObject * parent)
  : QAbstractListModel (parent)
{
  /*
   * add_sequence gets called from Logger, which in turn is called by algotihms
   * running in the processing thread; some GUI-related calls, however, need
   * to be done in the GUI (main) thread, so we use a signal
   */
  connect (this, &Repository::add_sequence, this,
           &Repository::add_sequence_slot, Qt::QueuedConnection);
}

Repository::~Repository ()
{
  clear ();
}

QHash<int, QByteArray>
Repository::roleNames () const
{
  QHash<int, QByteArray> roles;
  roles[NameRole] = "name";
  roles[TypeRole] = "type";
  return roles;
}

int
Repository::nodelist_count (NodeListProperty * list)
{
  Repository * r = qobject_cast<Repository *> (list->object);

  Q_ASSERT (r);
  return r->m_sequences.count ();
}

Sequence *
Repository::nodelist_at (NodeListProperty * list, int i)
{
  Repository * r = qobject_cast<Repository *> (list->object);

  Q_ASSERT (r);

  Sequence::ptr s = r->m_sequences.at (i);
  QQmlEngine::setObjectOwnership (s.data (), QQmlEngine::CppOwnership);
  return s.data ();
}

QVariant
Repository::data (const QModelIndex & index, int role) const
{
  Sequence::ptr s = m_sequences[index.row ()];

  if (role == NameRole)
    return s->name ();
  else if (role == TypeRole)
    return s->type ();
  return QVariant ();
}

int
Repository::rowCount (const QModelIndex &) const
{
  return m_sequences.count ();
}

void
Repository::add_item (const Item::ptr item, int frame,
                      Sequence::ItemType type,
                      const QString & sequence_name)
{
  Sequence::ptr sequence;

  for (Sequence::ptr sp : m_sequences)
    if (sp->name () == sequence_name)
      {
        sequence = sp;
        break;
      }
  // no sequence with requested name, need to create one
  if (!sequence)
    {
      sequence = Sequence::make (type, sequence_name);
      add_sequence (sequence);
    }
  sequence->add_item (frame, item);
}

Repository::NodeListProperty
Repository::nodes ()
{
  return NodeListProperty (this, nullptr, this->nodelist_count,
                           this->nodelist_at);
}

void
Repository::add_sequence_slot (Sequence::ptr sequence)
{
  int position = rowCount ();

  beginInsertRows (QModelIndex (), position, position);
  m_sequences << sequence;
  endInsertRows ();
  nodes_changed ();
}

void
Repository::clear ()
{
  removing_all_nodes ();
  beginResetModel ();
  m_sequences.clear ();
  nodes_changed ();
  endResetModel ();
}

void
Repository::dump_contents (const QString & filename)
{
  io::serialize_to_file (filename, m_sequences);
}

void
Repository::populate_from_file (const QString &filename)
{
  io::deserialize_from_file (filename, m_sequences);
}
}
