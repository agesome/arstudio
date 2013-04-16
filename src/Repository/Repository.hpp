#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <string>
#include <functional>
#include <memory>

#include <Sequence.hpp>

namespace Workspace
{
/**
        This class handles storage of data sequences in a tree-like structure,
        limited to one level of branches.

        All methods are designed to create a branch/sequence if it is not
        present in the tree.
 */

class Repository
{
public:
	typedef std::map <std::string, Sequence::ptr> sequenceMap;
	typedef std::pair <std::string, Sequence::ptr> mapItem;
	typedef std::map <std::string, sequenceMap> mapTree;

	typedef std::shared_ptr<Repository> ptr;

	std::function <void (std::string)> newBranchCallback; // < called when a new branch is added
	std::function <void (std::string)> branchRemovedCallback; // < called when a branch is removed
	std::function <void (std::string, std::string)> newSequenceCallback; // < called when a new sequence is added
	std::function <void (std::string, std::string)> sequenceRemovedCallback; // < called when a sequence is removed

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
