#include <stdlib.h>
#include <iostream>
#include <QApplication>

#include <repository.hpp>
#include <scenegraph.hpp>
#include <thread>

int
main (int argc, char * argv[])
{
	std::cout << __FUNCTION__ << std::endl;

// testing code for Sequence/Repository
	Workspace::Sequence::ptr sp;
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
	sp = boost::make_shared<Workspace::Sequence> (Workspace::ITEM_TYPE_PCLOUD);
	sp->addItem (2, boost::make_shared<Workspace::Item>());
	sp->addItem (3, boost::make_shared<Workspace::Item>());
	r.addSequence (12, sp);

	for (auto & it: r.getSequences ())
		{
			std::cout << "sequence type: \t" << it.second->getType () << std::endl;
			std::cout << "sequence id: \t" << it.first << std::endl;
			for (auto & i: it.second->getItems ())
				std::cout << "\titem id: " << i.first << std::endl;
		}

	std::cout << "List of item types:" << std::endl;
	for (int i = 0; i < Workspace::ITEM_TYPE_LEN_; i++)
		std::cout << "\t" << Workspace::itemTypeNames[i] << std::endl;

	unsigned char it = r.getItemTypes ();

	QApplication app (argc, argv);
	Workspace::Scenegraph s;

	std::cout << "Item types in repository:" << std::endl;
	for (int i = 0; i < Workspace::ITEM_TYPE_LEN_; i++)
		if (it & (1 << i))
			{
				std::cout << "\t" << Workspace::itemTypeNames[i] << std::endl;
				s.addCheckbox (i);
			}
	s.show ();

	std::thread load ([&r, &s]()
		{
			sleep (4);
			std::cout << "Loading data." << std::endl;
			for (auto & it: r.getSequences ())
				s.addSequence (it.second);
			std::cout << "Filtered sequences:" << std::endl;
			for (auto & it: s.	sequences)
				std::cout << "\t" << Workspace::itemTypeNames[it->getType ()] << std::endl;
		});
	load.detach ();

	return app.exec ();
}
