#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QAbstractListModel>
#include <QtDebug>

#include <Sequence.hpp>

namespace arstudio {
/**
 *      This class handles storage of data sequences in a tree-like
 * structure,
 *      limited to one level of branches.
 *
 *      All methods are designed to create a branch/sequence if it is not
 *      present in the tree.
 *
 *      TODO: make the default branch name configurable, can be nice for
 * logging
 */

class RepositoryNode
{
public:
  RepositoryNode (const QString & name,
                  Sequence::ptr data)
    : name_ (name), data_ (data)
  {
  }

  arstudio::Sequence *
  data (void) const
  {
    return data_.data ();
  }

  Sequence::ptr
  ptr (void) const
  {
    return data_;
  }

  QString
  name (void) const
  {
    return name_;
  }

  arstudio::Sequence::ItemType
  type (void) const
  {
    return data_->type ();
  }

private:
  QString       name_;
  Sequence::ptr data_;
};

class Repository : public QAbstractListModel
{
  Q_OBJECT
public:
  typedef QSharedPointer<Repository> ptr;
  enum NodeRoles
  {
    NameRole = Qt::UserRole + 1,
    TypeRole
  };

  Repository (QObject * parent = nullptr);

  static ptr
  make (QObject * parent = nullptr)
  {
    return ptr (new Repository (parent));
  }

  QVariant data (const QModelIndex & index, int role = NameRole) const;
  int rowCount (const QModelIndex & parent = QModelIndex ()) const;
  Q_INVOKABLE arstudio::Sequence * get (int index);

  void add_sequence (Sequence::ptr sequence, const QString & node_name);
  void add_item (Item::ptr item, unsigned int frame, Sequence::ItemType type,
                 const QString & node_name);
protected:
  QHash<int, QByteArray> roleNames () const;
private:
  QList<RepositoryNode> data_;
};
}

#endif // REPOSITORY_H
