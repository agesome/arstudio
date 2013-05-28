#include <Repository.hpp>


namespace arstudio {
Repository::Repository (QObject * parent)
  : QAbstractListModel (parent)
{
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
  const RepositoryNode & n = data_[index.row ()];

  if (role == NameRole)
    return n.name ();
  else if (role == TypeRole)
    return n.type ();
  return QVariant ();
}

int
Repository::rowCount (const QModelIndex &) const
{
  return data_.count ();
}

arstudio::Sequence *
Repository::get (int index)
{
  Sequence * ptr = data_[index].data ();
  // the sequence is managed by Repository. forbid QML from owning it,
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

  for (RepositoryNode & m : data_)
    if (m.name () == node_name)
      {
        s = m.ptr ();
        break;
      }
  if (!s)
    {
      int position = rowCount ();
      s = Sequence::make (type);
      RepositoryNode v (node_name, s);
      beginInsertRows (QModelIndex (), position, position);
      data_ << v;
      endInsertRows ();
    }
  s->add_item (frame, item);
}
}
