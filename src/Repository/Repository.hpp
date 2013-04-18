#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <string>
#include <functional>
#include <memory>

#include <Sequence.hpp>

namespace Workspace
{
/**
 *      This class handles storage of data sequences in a tree-like
 * structure,
 *      limited to one level of branches.
 *
 *      All methods are designed to create a branch/sequence if it is not
 *      present in the tree.
 *
 *      TODO: make the default branch name configurable, can be nice for
 *logging
 */

class Repository
{
public:
  typedef std::shared_ptr<Repository> ptr;

  typedef std::function <void (const std::string &)> branch_cb;
  typedef std::function <void (const std::string &,
                               const std::string &)> sequence_cb;

  void add_sequence (Sequence::ptr, const std::string &, const std::string &);
  void add_sequence (Sequence::ptr, const std::string &);
  const Sequence::ptr find_sequence (const std::string &name,
                                     const std::string &branch_name);
  const Sequence::ptr find_sequence (const std::string &);
  void add_item (Item::ptr, unsigned int, Item::type, const std::string &,
                 const std::string &);
  void add_item (Item::ptr, unsigned int, Item::type, const std::string &);
  void clear (void);
  static ptr make (void);

  branch_cb   new_branch_cb; //< called when a new branch is added
  branch_cb   removed_branch_cb; //< called when a branch is removed
  sequence_cb new_sequence_cb; //< called when a new sequence is added
  sequence_cb removed_sequence_cb; //< called when a sequence is removed
private:
  typedef std::map <std::string, Sequence::ptr> sequence_map;
  typedef std::pair <std::string, Sequence::ptr> map_item;
  typedef std::map <std::string, sequence_map> map_tree;

  sequence_map & get_sequence_map (const std::string &);

  map_tree sequences_;
};
}
#endif // REPOSITORY_H
