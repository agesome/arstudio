#include <sequence.hpp>

namespace Workspace
{

class Repository
{
public:
	std::map <int, seq_ptr> sequences;

	void addSequence (int, seq_ptr);
	// may have to switch to int with >8 item types
	unsigned char getItemTypes (void);
};

}