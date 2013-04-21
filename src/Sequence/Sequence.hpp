#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <map>
#include <string>
#include <memory>
#include <utility>

namespace arstudio {
/**
 * This is the base class for all data items.
 */

class Item
{
public:
  typedef std::shared_ptr<Item> ptr;

  /**
   * This method allows casting from a generic Item pointer to a
   * pointer to a specific Item subclass.
   *
   * \param item_ptr the pointer to be cast
   */

  template <class T>
  static inline std::shared_ptr<T>
  ptr_cast_to (const Item::ptr & item_ptr)
  {
    return std::dynamic_pointer_cast<T, Item> (item_ptr);
  }

  virtual
  ~Item ()
  {
  }

  enum type {
    CAMERA = 0,
    PCLOUD,
    POINT3D,
    BITMAP,

    LEN_
  };
};

/**
 * This class manages a frame-ordered sequence of data items.
 */

class Sequence
{
public:
  typedef std::shared_ptr<Sequence> ptr;
  typedef std::map <unsigned int, Item::ptr> frame_map;

  Sequence (Item::type);
  static ptr make (Item::type);
  void add_item (unsigned int, Item::ptr);
  const frame_map & items (void);
  Item::type type (void);

private:
  Item::type type_; //< type of items stored
  frame_map  items_; //< map of items to frames
};
}

#endif // SEQUENCE_H
