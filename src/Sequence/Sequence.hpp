#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <QObject>
#include <QSharedPointer>
#include <QQmlListProperty>
#include <QList>

#include <map>
#include <string>
#include <utility>

namespace arstudio {
/**
 * This is the base class for all data items.
 */

class Item
{
public:
  typedef QSharedPointer<Item> ptr;
  virtual
  ~Item () = default;
};

/**
 * This class manages a frame-ordered sequence of data items.
 */

class Sequence : public QObject
{
  Q_OBJECT
                        Q_ENUMS (ItemType)
public:
  typedef QSharedPointer<Sequence> ptr;
  typedef std::map <int, const Item::ptr> frame_map;
  enum ItemType { CAMERA, PCLOUD, POINT3D, BITMAP, INVALID };

  Sequence (ItemType, QObject * parent = nullptr);
  Sequence (QObject * parent = nullptr);

  static ptr make (ItemType);

  void add_item (int, const Item::ptr);
  const Item::ptr item_for_frame (int);
  const frame_map & items ();

  ItemType type ();
private:
  Q_DISABLE_COPY (Sequence)

  const ItemType m_type; //< type of items stored
  frame_map m_items;  //< map of items to frames
signals:
  void items_changed ();
};
}

#endif // SEQUENCE_H
