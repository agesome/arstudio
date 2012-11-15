#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <sequence.hpp>

namespace Workspace
{
class Repository
{
public:
	typedef std::pair <int, Sequence::ptr> pair;
	typedef std::map <int, Sequence::ptr> map;

	void addSequence (int, Sequence::ptr);
	void addItem (int, Item::type, int, Item::ptr);
	// may have to switch to int with >8 item types
	Item::typemask getItemTypes (void);
	const map & getSequences (void);
private:
	map sequences;
};
}

#endif // REPOSITORY_H