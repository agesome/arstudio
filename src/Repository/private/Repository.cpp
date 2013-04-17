#include <Repository.hpp>

namespace Workspace
{
/**
 *      This method adds a sequence to the specified branch.
 *
 *      \param sequence the sequence to be added
 *      \param id name under which to store the sequence
 *      \param branchName branch to which the sequence will be added,
 * defaults to "default"
 */

void
Repository::addSequence (Sequence::ptr sequence, std::string id,
                         std::string branchName)
{
  sequenceMap & mtr = getSequenceMap (branchName);

  mtr.insert (mapItem (id, sequence));
  if (newSequenceCallback)
    newSequenceCallback (id, branchName);
}

/**
 *      This method adds a single Item to a sequence.
 *
 *      \param item the item to be added
 *      \param nframe the frame to which this item corresponds
 *      \param type the type of the item
 *      \param id name the sequence to add to
 *      \param branchName branch under which the sequence is stored
 */

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
    it->second->addItem (nframe, item);
}

/**
 *      This method returns a sequenceMap for a branch.
 *
 *      \param branchName the branch to find
 */

Repository::sequenceMap &
Repository::getSequenceMap (std::string branchName = "default")
{
  mapTree::iterator it = sequences.find (branchName);

  if (it == sequences.end ())
    {
      auto treeItem = std::pair<std::string, sequenceMap> (branchName,
                                                           sequenceMap ());
      it = sequences.insert (treeItem).first;
      if (newBranchCallback)
        newBranchCallback (branchName);
      return it->second;
    }
  else
    return it->second;
}

Repository::ptr
Repository::make (void)
{
  return std::make_shared <Repository> ();
}

const Repository::mapTree &
Repository::getTree (void)
{
  return sequences;
}

/**
 *      This method removes all sequences from the repository.
 */

void
Repository::Clear (void)
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
