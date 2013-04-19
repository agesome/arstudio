#include <Repository.hpp>

namespace Workspace
{
/**
 *      This method adds a sequence to the specified branch.
 *
 *      \param seq the sequence to be added
 *      \param name how to name the sequence
 *      \param branch_name branch to which the sequence will be added
 */

void
Repository::add_sequence (Sequence::ptr seq, const std::string & name,
                          const std::string & branch_name)
{
  sequence_map & map = get_sequence_map (branch_name);

  map.insert (map_item (name, seq));
  if (new_sequence_cb)
    new_sequence_cb (name, branch_name);
}

void
Repository::add_sequence (Sequence::ptr seq, const std::string & name)
{
  return add_sequence (seq, name, "default");
}

/**
 * This method looks for a sequence in the Repository. If there is no such
 * sequence, an uninitialized Sequence::ptr is returned
 *
 * \param name name of the sequence
 * \param branch_name name of the branch
 */

const Sequence::ptr
Repository::find_sequence (const std::string & name,
                           const std::string & branch_name)
{
  Sequence::ptr result;

  sequence_map &         sm = get_sequence_map (branch_name);
  sequence_map::iterator it = sm.find (name);

  if (it != sm.end ())
    result = it->second;
  return result;
}

const Sequence::ptr
Repository::find_sequence (const std::string & name)
{
  return find_sequence (name, "default");
}

/**
 *      This method adds a single Item to a sequence.
 *
 *      \param item the item to be added
 *      \param frame the frame to which this item corresponds
 *      \param type the type of the item
 *      \param sequence_name name the sequence to add to
 *      \param branch_name branch under which the sequence is stored
 */

void
Repository::add_item (Item::ptr item, unsigned int frame, Item::type type,
                      const std::string & sequence_name,
                      const std::string & branch_name)
{
  Sequence::ptr seq = find_sequence (sequence_name, branch_name);

  if (!seq)
    {
      seq = Sequence::make (type);
      add_sequence (seq, sequence_name, branch_name);
    }
  seq->add_item (frame, item);
}

void
Repository::add_item (Item::ptr item, unsigned int frame, Item::type type,
                      const std::string & sequence_name)
{
  add_item (item, frame, type, sequence_name, "default");
}

/**
 *      This method returns a sequence_map for a branch.
 *
 *      \param branch_name the branch to find
 */

Repository::sequence_map &
Repository::get_sequence_map (const std::string & branch_name)
{
  map_tree::iterator it = sequences_.find (branch_name);

  if (it == sequences_.end ())
    {
      auto tree_item = std::pair<std::string, sequence_map> (branch_name,
                                                             sequence_map ());
      it = sequences_.insert (tree_item).first;
      if (new_branch_cb)
        new_branch_cb (branch_name);
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

/**
 *      This method removes all sequences from the repository.
 */

void
Repository::clear (void)
{
  std::string branch_name;

  for (auto sm : sequences_)
    {
      branch_name = sm.first;
      for (auto mi : sm.second)
        {
          removed_sequence_cb (mi.first, branch_name);
          sm.second.erase (mi.first);
        }
      removed_branch_cb (branch_name);
      sequences_.erase (branch_name);
    }
}
}
