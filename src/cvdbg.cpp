#include <stdlib.h>
#include <iostream>
#include <QApplication>

#include <repository.hpp>
#include <scenegraph.hpp>
#include <window3d.hpp>
#include <thread>
#include <point3d.hpp>

using namespace Workspace;

int
main (int argc, char * argv[])
{
	std::cout << __FUNCTION__ << std::endl;

// testing code for Sequence/Repository
	Sequence::ptr sp;
	Repository r;


// add some sequences
	sp = boost::make_shared<Sequence> (Item::CAMERA);
	sp->addItem (0, boost::make_shared<Item>());
	sp->addItem (1, boost::make_shared<Item>());
	r.addSequence (14, sp);
	sp = boost::make_shared<Sequence> (Item::FPVEC);
	sp->addItem (2, boost::make_shared<Item>());
	sp->addItem (3, boost::make_shared<Item>());
	r.addSequence (13, sp);
	sp = boost::make_shared<Sequence> (Item::PCLOUD);
	sp->addItem (2, boost::make_shared<Item>());
	sp->addItem (3, boost::make_shared<Item>());
	r.addSequence (12, sp);

	r.addItem (15, Item::FPVEC, 0, boost::make_shared<Item>());
	r.addItem (13, Item::FPVEC, 0, boost::make_shared<Item>());

for (auto & it: r.getSequences ())
		{
			std::cout << "sequence type: \t" << it.second->getType () << std::endl;
			std::cout << "sequence id: \t" << it.first << std::endl;
for (auto & i: it.second->getItems ())
				std::cout << "\titem id: " << i.first << std::endl;
		}

	std::cout << "List of item types:" << std::endl;
	for (int i = 0; i < Item::LEN_; i++)
		std::cout << "\t" << Item::typeNames[i] << std::endl;

	Item::typemask it = r.getItemTypes ();

	QApplication app (argc, argv);
	ScenegraphSelector::ptr s = boost::make_shared<ScenegraphSelector> ();
	Scenegraph g (s);

	std::cout << "Item types in repository:" << std::endl;
	for (int i = 0; i < Item::LEN_; i++)
		if (it & (1 << i))
			{
				std::cout << "\t" << Item::typeNames[i] << std::endl;
				s->addCheckbox (static_cast<Item::type>(i));
			}
	s->show ();

	<<<<<<< HEAD
//	std::thread load ([&r, &s, &g]()
//	{
//		sleep (4000);
	std::cout << "Loading data." << std::endl;
for (auto & it: r.getSequences ())
		g.addSequence (it.second);
	std::cout << "Filtered sequences:" << std::endl;
for (auto & it: g.getSequences ())
		std::cout << "\t" << Item::typeNames[it->getType ()] << std::endl;






//});
//	load.detach ();


	/******************************************************************************/
	//ScenegraphSelector::ptr ssTest = boost::make_shared<ScenegraphSelector> ();
	Scenegraph sgTest(s);
	Sequence::ptr seqTest;


	seqTest = boost::make_shared<Sequence> (Item::FPVEC);
	seqTest->addItem(0, boost::make_shared<Point3d>(0,0,0,0,0,0));
	seqTest->addItem(1, boost::make_shared<Point3d>(1,0,0,1,0,0));
	seqTest->addItem(2, boost::make_shared<Point3d>(0,1,0,0,0,1));
	seqTest->addItem(3, boost::make_shared<Point3d>());
	=======
	  std::thread load ([&r, &s, &g]()
	{
		sleep (4);
		std::cout << "Loading data." << std::endl;
for (auto & it: r.getSequences ())
			g.addSequence (it.second);
		std::cout << "Filtered sequences:" << std::endl;
for (auto & it: g.getSequences ())
			std::cout << "\t" << Item::typeNames[it->getType ()] << std::endl;
	});
	load.detach ();
	>>>>>>> d2aab5d55ba7046fd6ba950e7cb31dcf34284ff6

	sgTest.addSequence(seqTest);
	/****************************************************************************/
	qDebug()<<(int)s->getSelections();
	Window3D window3d (&g);
	window3d.show();

	return app.exec ();
}
