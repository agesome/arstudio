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

class Item : public QObject
{
  Q_OBJECT
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
  Q_PROPERTY (ItemType type READ type)
  Q_ENUMS (ItemType)
public:
  typedef QSharedPointer<Sequence> ptr;
  typedef std::map <unsigned int, Item::ptr> frame_map;
  enum ItemType { CAMERA, PCLOUD, POINT3D, BITMAP };

  Sequence (ItemType, QObject * parent = nullptr);
  Sequence (QObject * parent = nullptr);
  static ptr make (ItemType);
  void add_item (unsigned int, Item::ptr);
  const frame_map & items (void);
  ItemType type (void);
  Q_INVOKABLE arstudio::Item * item_for_frame (int);
private:
  ItemType  type_; //< type of items stored
  frame_map items_;  //< map of items to frames
};
}

#endif // SEQUENCE_H
