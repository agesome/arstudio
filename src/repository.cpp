#include <repository.hpp>

namespace Workspace
{
void
Repository::addSequence (Sequence::ptr p, std::string id,
                         std::string branch_name)
{
	branch & br = accessBranch (branch_name);

	br.insert (leaf (id, p));
	on_leaf_add (id, branch_name);
}

void
Repository::addItem (Item::ptr i, unsigned int nfr, Item::type type,
                     std::string id, std::string branch_name)
{
	branch & br = accessBranch (branch_name);

	branch::iterator it = br.find (id);

	if (it == br.end ())
		{
			Sequence::ptr p = Sequence::make (type);
			br.insert (leaf (id, p));
			on_leaf_add (id, branch_name);
			p->addItem (nfr, i);
		}
	else
		{
			it->second->addItem (nfr, i);
		}
}

Repository::branch &
Repository::accessBranch (std::string branch_name = "default")
{
	tree::iterator it = sequences.find (branch_name);

	if (it == sequences.end ())
		{
			it = sequences.insert (tree_pair (branch_name, branch ())).first;
			on_branch_add (branch_name);
			return it->second;
		}
	else
		{
			return it->second;
		}
}

Repository::ptr
Repository::make (void)
{
	return boost::make_shared <Repository> ();
}

const Repository::tree &
Repository::getTree (void)
{
	return sequences;
}
}
