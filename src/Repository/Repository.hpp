#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QAbstractListModel>
#include <QQmlEngine>

#include <Sequence.hpp>
#include <Logger.hpp>

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
    : m_name (name), m_data (data)
  {
  }

  RepositoryNode () = default;

  arstudio::Sequence *
  data (void) const
  {
    return m_data.data ();
  }

  Sequence::ptr
  ptr (void) const
  {
    return m_data;
  }

  QString
  name (void) const
  {
    return m_name;
  }

  arstudio::Sequence::ItemType
  type (void) const
  {
    return m_data->type ();
  }

private:
  QString       m_name;
  Sequence::ptr m_data;
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
  ~Repository (void);

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
  QList<RepositoryNode> m_nodes;

  /*
   * add_item ends up being called from the processing thread,
   * but QAbstractListModel is not thread-safe, so do we append
   * in the main thread
   */
signals:
  void removing_all_nodes (void);
  void append_node_signal (const RepositoryNode & node);
private slots:
  void append_node_slot (const RepositoryNode & node);
public slots:
  void clear (void);
};
}

Q_DECLARE_METATYPE (arstudio::RepositoryNode)

#endif // REPOSITORY_H
