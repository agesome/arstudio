#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QAbstractListModel>
#include <QQmlEngine>
#include <QQmlListProperty>

#include <RepositoryIO.hpp>

namespace arstudio {
/**
 * @brief Sequence storage class
 *
 * This class serves as storage for all data processed by Logger. At the same
 * time, Repository provieds a QAbstractListModel reflecting it's contents to
 * be viewed by the user. Sequences are stored in RepositoryNode instances to
 * allow convenient access from views
 */

class Repository : public QAbstractListModel
{
  Q_OBJECT
  Q_PROPERTY (QQmlListProperty<arstudio::Sequence> nodes READ nodes
              NOTIFY nodes_changed)
public:
  typedef QSharedPointer<Repository> ptr;
  typedef QQmlListProperty<arstudio::Sequence> NodeListProperty;

  enum NodeRoles
  {
    NameRole = Qt::UserRole + 1,
    TypeRole
  };

  Repository (QObject * parent = nullptr);
  ~Repository ();

  static ptr
  make (QObject * parent = nullptr)
  {
    return ptr (new Repository (parent));
  }

  QVariant data (const QModelIndex & index, int role = NameRole) const;
  int rowCount (const QModelIndex & parent = QModelIndex ()) const;

  /**
   * @brief Add an item to already stored sequence
   *
   * If specified sequence does not exist, it is created
   *
   * @param item the item to store
   * @param frame the frame to which this item corresponds
   * @param type type of the item
   * @param sequence_name name of the destination sequence
   */
  void add_item (const Item::ptr item, int frame, Sequence::ItemType type,
                 const QString & sequence_name);

  NodeListProperty nodes ();
protected:
  QHash<int, QByteArray> roleNames () const;
private:
  Q_DISABLE_COPY (Repository)
  QList<Sequence::ptr> m_sequences;

  static int nodelist_count (NodeListProperty *);
  static arstudio::Sequence * nodelist_at (NodeListProperty *, int);

  /*
   * add_item ends up being called from the processing thread,
   * but QAbstractListModel is not thread-safe, so do we append
   * in the main thread
   */
signals:
  void removing_all_nodes ();
  void add_sequence (Sequence::ptr sequence);
  void nodes_changed ();
private slots:
  void add_sequence_slot (Sequence::ptr sequence);
public slots:
  void clear ();
  void dump_contents (const QString & filename);
  void populate_from_file (const QString & filename);
};
}
#endif // REPOSITORY_H
