#include <sequence.hpp>

namespace Workspace
{

class Repository
{
public:
	std::map < int, seq_ptr > sequences;

	void addSequence (int, seq_ptr);
};

}