#ifndef SEQUENCE_H
#define SEQUENCE_H
#include <map>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace Workspace
{

class Item
{
public:
	typedef boost::shared_ptr<Item> ptr;
	typedef std::pair<int, ptr> pair;
};

enum item_type
{
	ITEM_TYPE_CAMERA,
	ITEM_TYPE_PCLOUD,
	ITEM_TYPE_FPVEC,

	ITEM_TYPE_LEN_
};

static std::string itemTypeNames [ITEM_TYPE_LEN_] = {
	std::string ("Camera"),
	std::string ("Point cloud"),
	std::string ("FP Vector")
};

class Sequence
{
public:
	typedef boost::shared_ptr<Sequence> ptr;
	typedef std::pair<int, ptr> pair;
	typedef std::map <int, Item::ptr> map;

	Sequence (item_type);
	void addItem (int, Item::ptr);
	const map & getItems (void);
	item_type getType (void);

private:
	item_type type;
	map items;
};

}
#endif // SEQUENCE_H
