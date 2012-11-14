#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/make_shared.hpp>
#include <sequence.hpp>
#include <scenegraph_selector.hpp>

namespace Workspace
{

class Scenegraph
{
public:
	typedef std::list<Sequence::ptr> list;

    Scenegraph (ScenegraphSelector *);
	void addSequence (Sequence::ptr);
	const list & getSequences (void);
private:
	list sequences;
    ScenegraphSelector * selector;
};

}
#endif // SCENEGRAPH_H
