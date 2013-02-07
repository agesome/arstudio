#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <sequence.hpp>
#include <string>
#include <functional>

namespace Workspace
{
class Repository
{
public:
	typedef std::pair <std::string, Sequence::ptr> leaf;
	typedef std::map <std::string, Sequence::ptr> branch;
	typedef std::map <std::string, branch> tree;
	typedef std::pair <std::string, branch> tree_pair;
	typedef boost::shared_ptr <Repository> ptr;

	std::function <void (std::string)> on_branch_add;
	std::function <void (std::string, std::string)> on_leaf_add;

	void addSequence (Sequence::ptr, std::string, std::string = "default");
	void addItem (Item::ptr, unsigned int, Item::type, std::string,
	              std::string = "default");
	static ptr make (void);
	const tree & getTree (void);
	const Sequence::ptr getSequence (std::string, std::string);
	branch & accessBranch (std::string);
private:
	tree sequences;
};
}

#endif // REPOSITORY_H
