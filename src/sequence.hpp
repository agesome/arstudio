#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace Sequence
{

class Item {};
typedef std::pair< int, boost::shared_ptr<Item> > item_mmap_t;

enum
{
	ITEM_TYPE_CAMERA
};

class Sequence
{
public:
	std::multimap< int, boost::shared_ptr<Item> > items;

	void addItem (int type, boost::shared_ptr<Item> p);
};

}