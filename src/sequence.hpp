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
	typedef unsigned char typemask;

	enum type
	{
	    CAMERA,
	    PCLOUD,
	    FPVEC,

	    LEN_
	};

	static const std::string typeNames[LEN_];
};

class Sequence
{
public:
	typedef boost::shared_ptr<Sequence> ptr;
	typedef std::pair<int, ptr> pair;
	typedef std::map <int, Item::ptr> map;

	Sequence (Item::type);
	static ptr make (Item::type);
	void addItem (int, Item::ptr);
	const map & getItems (void);
	Item::type getType (void);

private:
	Item::type type;
	map items;
};

}
#endif // SEQUENCE_H
