#include <stdlib.h>
#include <iostream>
#include <QApplication>

#include <repository.hpp>
#include <scenegraph.hpp>

int
main (int argc, char * argv[])
{
	std::cout << __FUNCTION__ << std::endl;

// testing code for Sequence/Repository
	Workspace::seq_ptr sp;
	Workspace::Repository r;

// add some sequences
	sp = boost::make_shared<Workspace::Sequence> (Workspace::ITEM_TYPE_CAMERA);
	sp->addItem (0, boost::make_shared<Workspace::Item>());
	sp->addItem (1, boost::make_shared<Workspace::Item>());
	r.addSequence (14, sp);
	sp = boost::make_shared<Workspace::Sequence> (Workspace::ITEM_TYPE_FPVEC);
	sp->addItem (2, boost::make_shared<Workspace::Item>());
	sp->addItem (3, boost::make_shared<Workspace::Item>());
	r.addSequence (13, sp);

	for (auto it: r.sequences)
		{
			std::cout << "sequence type: \t" << it.second->type << std::endl;
			std::cout << "sequence id: \t" << it.first << std::endl;
			for (auto i: it.second->items)
				std::cout << "\titem id: " << i.first << std::endl;
		}

	std::cout << "List of item types:" << std::endl;
	for (int i = 0; i < Workspace::ITEM_TYPE_LEN_; i++)
		std::cout << "\t" << Workspace::itemTypeNames[i] << std::endl;

	unsigned char it = r.getItemTypes ();

	std::list<std::string> sc_args;
	std::cout << "Item types in repository:" << std::endl;
	for (int i = 0; i < Workspace::ITEM_TYPE_LEN_; i++)
		if (it & (1 << i))
			{
				std::cout << "\t" << Workspace::itemTypeNames[i] << std::endl;
				sc_args.push_back (Workspace::itemTypeNames[i]);
			}

	QApplication app (argc, argv);
	Workspace::Scenegraph s (sc_args, NULL);
	s.show ();
	return app.exec ();
}
