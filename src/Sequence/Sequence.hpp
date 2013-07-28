#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <QObject>
#include <QSharedPointer>
#include <QMap>

namespace arstudio {
/**
 * @brief Base class for all item types
 */

class Item
{
public:
  typedef QSharedPointer<Item> ptr;
  virtual
  ~Item () = default;
};

/**
 * @brief Manages a frame-ordered sequence of data items
 */

class Sequence : public QObject
{
  Q_OBJECT
                        Q_ENUMS (ItemType)
public:
  typedef QSharedPointer<Sequence> ptr;
  typedef QMap<int, Item::ptr> FrameMap;

  enum ItemType { Invalid = 0, Bitmap, Camera, Point, PointCloud };

  Sequence (ItemType, QObject * parent = nullptr);
  Sequence (QObject * parent = nullptr);

  static ptr make (ItemType);

  /**
   * @brief Add an item for a specific frame
   */
  void add_item (int frame, const Item::ptr item_ptr);
  /**
   * @brief Get an item for a specific frame
   * @return Item::ptr for the item, uninitialized if item was not found
   */
  const Item::ptr item_for_frame (int frame);
  const FrameMap & items ();

  ItemType type ();
private:
  Q_DISABLE_COPY (Sequence)

  const ItemType m_type; //< type of items stored
  FrameMap m_items;  //< map of items to frames
signals:
  void items_changed ();
};
}

#endif // SEQUENCE_H
