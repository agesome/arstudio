#include <stdlib.h>
#include <iostream>
#include <QApplication>

#include <repository.hpp>
#include <scenegraph.hpp>
#include <window3d.hpp>
#include <thread>
#include <point3d.hpp>
#include "camera.hpp"
using namespace Workspace;

int
main (int argc, char * argv[])
{
	QApplication app (argc, argv);

	std::cout << __FUNCTION__ << std::endl;

// testing code for Sequence/Repository
	Sequence::ptr sp;
	Repository r;


// add some sequences

	for (int i=1; i<11; i++)
		{

			//   boost::shared_ptr<Point3d> p2 = boost::static_pointer_cast<Point3d>(itemmm);

			//   Item::ptr itemm2 = boost::make_shared<Point3d>();
			boost::shared_ptr<Point3d> p2=boost::make_shared<Point3d>();;
			p2->x=i*0.1;
			p2->y=0;
			p2->z=0;
			p2->r=0;
			p2->g=0;
			p2->b=0;

			boost::shared_ptr<Camera> p1=boost::make_shared<Camera>();;
			p1->tx=0.05*i;
			p1->ty=0;
			p1->tz=0;

			r.addItem (15, Item::FPVEC, i, p2);
			r.addItem (13, Item::CAMERA, i,p1);



		}


for (auto & it: r.getSequences ())
		{
			std::cout << "sequence type: \t" << it.second->getType () << std::endl;
			std::cout << "sequence id: \t" << it.first << std::endl;
for (auto & i: it.second->getItems ())
				std::cout << "\titem nframe: " << i.first << std::endl;
		}

	std::cout << "List of item types:" << std::endl;
	for (int i = 0; i < Item::LEN_; i++)
		std::cout << "\t" << Item::typeNames[i] << std::endl;

	Item::typemask it = r.getItemTypes ();

	ScenegraphSelector::ptr s = ScenegraphSelector::make ();
	Scenegraph::ptr g = Scenegraph::make (s);

	std::cout << "Item types in repository:" << std::endl;
	for (int i = 0; i < Item::LEN_; i++)
		if (it & (1 << i))
			{
				std::cout << "\t" << Item::typeNames[i] << std::endl;
				s->addCheckbox (static_cast<Item::type>(i));
			}
	s->show ();



	std::cout << "Loading data." << std::endl;
for (auto & it: r.getSequences ())
		g->addSequence (it.second);
	std::cout << "Filtered sequences:" << std::endl;
for (auto & it: g->getSequences ())
		std::cout << "\t" << Item::typeNames[it->getType ()] << std::endl;





	Window3D window3d (g);
	window3d.show();

	return app.exec ();
}
