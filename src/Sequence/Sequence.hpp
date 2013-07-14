#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <QObject>
#include <QSharedPointer>
#include <QQmlListProperty>
#include <QList>

#include <map>
#include <string>
#include <utility>

#include <repository.pb.h>

namespace ap = arstudio_protobuf;

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
                        Q_ENUMS (ItemTypes)
public:
  typedef QSharedPointer<Sequence> ptr;
  typedef std::map <int, const Item::ptr> frame_map;
  /*
   * This MUST mirror the Node::Type struct from repository.proto !
   * Since we cannot properly expose the Node_Type struct to QML,
   * because it is in protobuf-generated non-Qt code
   */
  enum ItemTypes { Invalid = 0, Bitmap, Camera, Point, PointCloud };


  Sequence (ap::Node::Type, QObject * parent = nullptr);
  Sequence (QObject * parent = nullptr);

  static ptr make (ap::Node::Type);

  /**
   * @brief Add an item for a specific frame
   */
  void add_item (int frame, const Item::ptr item_ptr);
  /**
   * @brief Get an item for a specific frame
   * @return Item::ptr for the item, uninitialized if item was not found
   */
  const Item::ptr item_for_frame (int frame);
  const frame_map & items ();

  ap::Node::Type type ();
private:
  Q_DISABLE_COPY (Sequence)

  const ap::Node::Type m_type; //< type of items stored
  frame_map m_items;  //< map of items to frames
signals:
  void items_changed ();
};
}

#endif // SEQUENCE_H
