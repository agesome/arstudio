#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <map>
#include <string>
#include <memory>

namespace Workspace
{
/**
        This is the base class for all data items.
 */

class Item
{
public:
	typedef std::shared_ptr<Item> ptr;
	typedef std::pair<int, ptr> pair;

	/**
	        This method allows casting from a generic Item pointer to a pointer to
	        a specific Item type.

	        \param r the pointer to be cast
	 */

	template <class T> static std::shared_ptr<T> ptr_cast_to (const Item::ptr & r)
	{
		return std::dynamic_pointer_cast<T, Item> (r);
	}

	virtual ~Item ()
	{
	}

	enum type {
		CAMERA = 0,
		PCLOUD,
		POINT3D,
		BITMAP,

		LEN_
	};

	static const std::string typeNames[LEN_]; // << holds human-readable type names for all item types
};

/**
        This class manages a frame-ordered sequence of data items.
 */

class Sequence
{
public:
	typedef std::shared_ptr<Sequence> ptr;
	typedef std::pair<unsigned int, ptr> pair;
	typedef std::map <unsigned int, Item::ptr> map;

	Sequence (Item::type);
	static ptr make (Item::type);
	void addItem (int, Item::ptr);
	const map & getItems (void);
	Item::type getType (void);

private:
	Item::type type; // < type of items stored
	map items; // < map of items to frames
};
}
#endif // SEQUENCE_H
