#include <Repository.hpp>

namespace arstudio {
Repository::Repository (QObject * parent)
  : QAbstractListModel (parent)
{
  connect (this, &Repository::append_node_signal,
           this, &Repository::append_node_slot,
           Qt::QueuedConnection);
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
  return r->m_nodes.count ();
}

RepositoryNode *
Repository::nodelist_at (NodeListProperty * list, int i)
{
  Repository * r = qobject_cast<Repository *> (list->object);

  Q_ASSERT (r);

  RepositoryNode * n = r->m_nodes.at (i);
  QQmlEngine::setObjectOwnership (n, QQmlEngine::CppOwnership);
  return n;
}

QVariant
Repository::data (const QModelIndex & index, int role) const
{
  RepositoryNode * n = m_nodes[index.row ()];

  if (role == NameRole)
    return n->name ();
  else if (role == TypeRole)
    return n->type ();
  return QVariant ();
}

int
Repository::rowCount (const QModelIndex &) const
{
  return m_nodes.count ();
}

void
Repository::add_item (const Item::ptr item, int frame,
                      Sequence::ItemType type,
                      const QString & node_name)
{
  Sequence::ptr s;

  for (RepositoryNode * m : m_nodes)
    if (m->name () == node_name)
      {
        s = m->shared_ptr ();
        break;
      }
  if (!s)
    {
      s = Sequence::make (type);
      append_node_signal (new RepositoryNode (node_name, s));
    }
  s->add_item (frame, item);
}

Repository::NodeListProperty
Repository::nodes ()
{
  return NodeListProperty (this,
                           nullptr,
                           this->nodelist_count,
                           this->nodelist_at);
}

void
Repository::append_node_slot (RepositoryNode * node)
{
  int position = rowCount ();

  beginInsertRows (QModelIndex (), position, position);
  m_nodes << node;
  endInsertRows ();
  nodes_changed ();
}

void
Repository::clear ()
{
  removing_all_nodes ();
  beginResetModel ();
  for (RepositoryNode * n : m_nodes)
    delete n;
  m_nodes.clear ();
  nodes_changed ();
  endResetModel ();
}

void
Repository::dump_contents (const QString & filename)
{
  io::serialize_to_file (filename, m_nodes);
}

void
Repository::populate_from_file (const QString &filename)
{
  io::deserialize_from_file (filename, m_nodes);
}
}
