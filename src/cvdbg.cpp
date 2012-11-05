#include <stdlib.h>
#include <iostream>

#include <repository.hpp>

int
main (int argc, char * argv[])
{
	std::cout << __FUNCTION__ << std::endl;

// testing code for Sequence/Repository
	Workspace::seq_ptr sp (new Workspace::Sequence (Workspace::ITEM_TYPE_CAMERA));
	Workspace::Repository r;

	sp->addItem (42, boost::make_shared<Workspace::Item>());
	r.addSequence (13, sp);
	for (auto it: r.sequences)
		{
			std::cout << "sequence type: \t" << it.second->type << std::endl;
			std::cout << "sequence id: \t" << it.first << std::endl;
			for (auto i: it.second->items)
				std::cout << "\titem id: " << i.first << std::endl;
		}
	return EXIT_SUCCESS;
}