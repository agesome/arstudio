#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <list>
#include <sequence.hpp>
#include <climits>

namespace Workspace
{
class Scenegraph
{
public:
	typedef boost::shared_ptr<Scenegraph> ptr;
	typedef std::list<Sequence::ptr> list;

	static ptr make ();
	void addSequence (Sequence::ptr);
	void removeSequence (Sequence::ptr);
	const list & getSequences (void);
	unsigned int getMinFrame (void);
	unsigned int getMaxFrame (void);
private:
	list sequences;
};
}
#endif // SCENEGRAPH_H
