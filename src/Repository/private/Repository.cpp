#include <Repository.hpp>

namespace Workspace
{
void
Repository::addSequence (Sequence::ptr sequence, std::string id,
                         std::string branchName)
{
	sequenceMap & mtr = getSequenceMap (branchName);

	mtr.insert (mapItem (id, sequence));
	if (newSequenceCallback)
		newSequenceCallback (id, branchName);
}

void
Repository::addItem (Item::ptr item, unsigned int nframe, Item::type type,
                     std::string id, std::string branchName)
{
	sequenceMap & sm = getSequenceMap (branchName);

	sequenceMap::iterator it = sm.find (id);

	if (it == sm.end ())
		{
			Sequence::ptr p = Sequence::make (type);
			sm.insert (mapItem (id, p));
			if (newSequenceCallback)
				newSequenceCallback (id, branchName);
			p->addItem (nframe, item);
		}
	else
		{
			it->second->addItem (nframe, item);
		}
}

Repository::sequenceMap &
Repository::getSequenceMap (std::string branchName = "default")
{
	mapTree::iterator it = sequences.find (branchName);

	if (it == sequences.end ())
		{
			auto treeItem = std::pair<std::string, sequenceMap> (branchName, sequenceMap ());
			it = sequences.insert (treeItem).first;
			if (newBranchCallback)
				newBranchCallback (branchName);
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

const Repository::mapTree &
Repository::getTree (void)
{
	return sequences;
}

void Repository::Clear (void)
{
	std::string branchName;

	for (auto sm : sequences)
		{
			branchName = sm.first;
			for (auto mi : sm.second)
				{
					sequenceRemovedCallback (branchName, mi.first);
					sm.second.erase (mi.first);
				}
			branchRemovedCallback (branchName);
			sequences.erase (branchName);
		}
}
}