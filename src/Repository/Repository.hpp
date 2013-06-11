#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QAbstractListModel>
#include <QQmlEngine>

#include <Sequence.hpp>

namespace arstudio {
/*
 * This class describes a single item in the Repository model
 */

class RepositoryNode : public QObject
{
  Q_OBJECT
  Q_PROPERTY (arstudio::Sequence::ItemType type READ type CONSTANT)
  Q_PROPERTY (arstudio::Sequence * ptr READ data CONSTANT)
public:
  RepositoryNode (const QString & name,
                  Sequence::ptr data,
                  QObject * parent = nullptr)
    :
    QObject (parent),
    m_name (name),
    m_data (data)
  {
  }

  RepositoryNode (void) = default;

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

/**
 * Sequence storage class
 *
 * This class serves as storage for all data processed by Logger. At the
 * same
 * time, Repository provieds a QAbstractListModel reflecting it's contents
 * to be viewed by the user. Sequences are stored in RepositoryNodes for
 * convenient access from views.
 */

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
  Q_INVOKABLE arstudio::RepositoryNode * get (int index);

  /**
   * Add a sequence to the list by it's Sequence::ptr
   * @param sequence the pointer
   * @param node_name name under which to store the sequence
   */
  void add_sequence (Sequence::ptr sequence, const QString & node_name);
  /**
   * Add an item to already stored sequence, or create a sequence for this
   * item if there isn't one yet.
   * @param item the item to store
   * @param frame the frame to which this item corresponds
   * @param type type of the item
   * @param node_name name of the destination sequence
   */
  void add_item (Item::ptr item, unsigned int frame, Sequence::ItemType type,
                 const QString & node_name);
protected:
  QHash<int, QByteArray> roleNames () const;
private:
  QList<RepositoryNode *> m_nodes;

  /*
   * add_item ends up being called from the processing thread,
   * but QAbstractListModel is not thread-safe, so do we append
   * in the main thread
   */
signals:
  void removing_all_nodes (void);
  void append_node_signal (RepositoryNode * node);
private slots:
  void append_node_slot (RepositoryNode * node);
public slots:
  void clear (void);
};
}
#endif // REPOSITORY_H
