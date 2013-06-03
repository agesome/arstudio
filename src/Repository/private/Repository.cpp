#include <Repository.hpp>

namespace arstudio {
Repository::Repository (QObject * parent)
  : QAbstractListModel (parent)
{
  connect (this, &Repository::append_node_signal,
           this, &Repository::append_node_slot,
           Qt::QueuedConnection);
}

Repository::~Repository (void)
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

QVariant
Repository::data (const QModelIndex & index, int role) const
{
  const RepositoryNode * n = m_nodes[index.row ()];

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

arstudio::RepositoryNode *
Repository::get (int index)
{
  RepositoryNode * ptr = m_nodes[index];
  // the RepositoryNode is managed by Repository. forbid QML from owning
  // it,
  // to prevent grabage collection
  QQmlEngine::setObjectOwnership (ptr, QQmlEngine::CppOwnership);

  return ptr;
}

void
Repository::add_item (Item::ptr item, unsigned int frame,
                      Sequence::ItemType type,
                      const QString & node_name)
{
  Sequence::ptr s;

  for (RepositoryNode * m : m_nodes)
    if (m->name () == node_name)
      {
        s = m->ptr ();
        break;
      }
  if (!s)
    {
      s = Sequence::make (type);
      append_node_signal (new RepositoryNode (node_name, s));
    }
  s->add_item (frame, item);
}

void
Repository::append_node_slot (RepositoryNode * node)
{
  int position = rowCount ();

  beginInsertRows (QModelIndex (), position, position);
  m_nodes << node;
  endInsertRows ();
}

void
Repository::clear (void)
{
  removing_all_nodes ();
  beginResetModel ();
  // Sequence::ptr go out of scope, sequences are freed
  m_nodes.clear ();
  endResetModel ();
}
}
