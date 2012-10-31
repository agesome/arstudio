#include <stdlib.h>
#include <iostream>

#include <sequence.hpp>
#include <repository.hpp>

int
main (int argc, char * argv[])
{
	std::cout << __FUNCTION__ << std::endl;

// testing code for Sequence
	Sequence::Sequence s;

	s.addItem (Sequence::ITEM_TYPE_CAMERA, boost::make_shared<Sequence::Item>());
	for (auto it: s.items)
		std::cout << it.first << std::endl;

	return EXIT_SUCCESS;
}