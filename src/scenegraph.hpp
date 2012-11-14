#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/make_shared.hpp>
#include <sequence.hpp>
#include <scenegraph_selector.hpp>
#include <climits>

namespace Workspace
{

class Scenegraph
{
public:
	typedef boost::shared_ptr<Scenegraph> ptr;
	typedef std::list<Sequence::ptr> list;

    Scenegraph (ScenegraphSelector *);
    static ptr make (ScenegraphSelector *);
	void addSequence (Sequence::ptr);
	const list & getSequences (void);
	unsigned int getMinFrame (void);
	unsigned int getMaxFrame (void);
private:
	list sequences;
    ScenegraphSelector * selector;
};

}
#endif // SCENEGRAPH_H
