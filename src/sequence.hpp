#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace Workspace
{

class Item {};
typedef boost::shared_ptr<Item> item_ptr;
typedef std::pair<int, item_ptr> item_map_t;

enum item_type
{
	ITEM_TYPE_CAMERA
};

class Sequence
{
public:
	item_type type;
	std::map < int, item_ptr > items;

	Sequence (item_type);
	void addItem (int, item_ptr);
};

typedef boost::shared_ptr<Sequence> seq_ptr;
typedef std::pair<int, seq_ptr> seq_map_t;

}