#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <string>
#include <functional>

#include <Sequence.hpp>

namespace Workspace
{
class Repository
{
public:
	typedef std::map <std::string, Sequence::ptr> sequenceMap;
	typedef std::pair <std::string, Sequence::ptr> mapItem;
	typedef std::map <std::string, sequenceMap> mapTree;

	typedef boost::shared_ptr <Repository> ptr;

	std::function <void (std::string)> newBranchCallback;
	std::function <void (std::string)> branchRemovedCallback;
	std::function <void (std::string, std::string)> newSequenceCallback;
	std::function <void (std::string, std::string)> sequenceRemovedCallback;

	void addSequence (Sequence::ptr, std::string, std::string = "default");
	void addItem (Item::ptr, unsigned int, Item::type, std::string,
	              std::string = "default");
	void Clear (void);
	static ptr make (void);
	const mapTree & getTree (void);
	const Sequence::ptr getSequence (std::string, std::string);
	sequenceMap & getSequenceMap (std::string);
private:
	mapTree sequences;
};
}
#endif // REPOSITORY_H
