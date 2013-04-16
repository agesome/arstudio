#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <list>
#include <climits>
#include <memory>

#include <Sequence.hpp>

namespace Workspace
{
/**
        Keeps track of all sequences to be displayed.
 */
class Scenegraph
{
public:
	typedef std::shared_ptr<Scenegraph> ptr;
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
