#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <list>
#include <climits>
#include <memory>

#include <Sequence.hpp>

namespace arstudio {
/**
 *      Keeps track of all sequences to be displayed.
 */
class Scenegraph
{
public:
  typedef std::shared_ptr<Scenegraph> ptr;
  typedef std::list<Sequence::ptr> sequence_list;

  static ptr make ();
  void add_sequence (Sequence::ptr);
  void remove_sequence (Sequence::ptr);
  const sequence_list & sequences (void);
  unsigned int min_frame (void);
  unsigned int max_frame (void);
private:
  sequence_list sequences_;
};
}

#endif // SCENEGRAPH_H
