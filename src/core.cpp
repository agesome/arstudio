#include <core.hpp>

Core::Core() :
	QMainWindow ()
{
	initGUI ();
	connect (tmlnmod, SIGNAL (newFrame (int)), wnd3d, SLOT (update (int)));

	apipe->loadModules ();
	cview->populate ();

	// std::function<void(const Config::tree_t &, int)> walk =
	// [&walk, this](const Config::tree_t & tree, int level)
	// {
	// for (auto it: tree)
	// {
	// for (int i = level; i > 0; i--)
	// std::cout << "\t";
	// std::cout << it.first << std::endl;
	// if (!it.second.empty ())
	// walk (it.second, level + 1);
	// }
	// };

	// walk (Config::getInstance ().rawTree (), 0);

	// std::ifstream f ("../data/ny0.txt");
	// while (f.good ())
	// {
	// double x, y, z, r, g, b;
	// f >> x >> y >> z >> r >> g >> b;
	// ::Point3d::ptr point = ::Point3d::make (x, y, z, r, g, b);
	// result->cloud.push_back (point);
	// }
	// repo->addItem (result, 1, Item::PCLOUD, "image");

	// Logger::setRepository (repo);
	// cv::VideoCapture capture ("video.mkv");
	// cv::Mat image, empty;
	// capture.set (CV_CAP_PROP_POS_MSEC, 212000.0);
	// PointCloud::ptr result = PointCloud::make ();

	// for (int i = 0; i < 10; i++)
	// {
	// capture >> image;
	// apipe->processFrame (image, empty);
	// }

	for (int i = 1; i < 42; i++)
		{
			::Point3d::ptr p2 = ::Point3d::make ();
			p2->x = i * 0.1;
			p2->y = 0;
			p2->z = 0;
			p2->r = 0;
			p2->g = 0;
			p2->b = 0;


			Camera::ptr p1 = Camera::make ();
			p1->tx = 0;
			p1->ty = 0;
			p1->tz = 0.05 * i;
			p1->rx = 5 * i;
			p1->ry = 0;
			p1->rz = 0;

			repo->addItem (p2, i, Item::POINT3D, "point");
			repo->addItem (p1, i, Item::CAMERA, "camera");
		}
	// for (auto it : repo->getTree ())
	// {
	// std::cout << "branch: " << it.first << std::endl;
	// for (auto l : it.second)
	// std::cout << "\t leaf: " << l.first << std::endl;
	// }
	tmlnmod->setMax (scgr->getMaxFrame ());
	tmln->updateWidget ();
}
void Core::quit ()
{
	qApp->exit ();
}


void Core::initGUI ()
{
	this->setGeometry (10, 10, 800, 500);
	this->setCentralWidget (central);
	repo_view->setMaximumWidth (180);
	menu_file->addAction ("Open", this, SLOT (open ()));
	menu_file->addSeparator ();
	menu_file->addAction ("Exit", this, SLOT (quit ()));
	menu_edit->addAction ("Settings", this, SLOT (settings ()));
	menu_help->addAction ("Help", this, SLOT (about ()), Qt::Key_F1);
	menu_help->addSeparator ();
	menu_help->addAction ("About", this, SLOT (about ()));
	mnuBar->addMenu (menu_file);
	mnuBar->addMenu (menu_edit);
	mnuBar->addMenu (menu_help);
	this->setMenuBar (mnuBar);

	layout->addWidget (mainsplitter, 0, 0, 1, 1);
	wnd3d->setMinimumSize (300, 300);
	winsplitter->addWidget (wnd3d);
	winsplitter->addWidget (tmln);
	mainsplitter->addWidget (repo_view);
	mainsplitter->addWidget (winsplitter);
	mainsplitter->addWidget (cview);
}


void Core::open ()
{
	qDebug () << "Open file";
}


void Core::about ()
{
	qDebug () << "About";
}


void Core::settings ()
{
	qDebug () << "Settings";
}


void Core::help ()
{
	qDebug () << "Help";
}
