#include <core.hpp>

Core::Core() :
	QMainWindow ()
{
	central = new QWidget (this);
	this->setObjectName (QString::fromUtf8 ("Core"));
	this->setGeometry (10, 10, 800, 500);
	this->setCentralWidget (central);
    layout = new QGridLayout (central);
    mainsplitter = new QSplitter (Qt:: Horizontal, central);
    winsplitter = new QSplitter (Qt:: Vertical, central);
    scgr = Workspace::Scenegraph::make ();
    wnd3d = new Window3D (scgr, central);
    tmlnmod = new TimeLineModel(1, 10);
    tmln = new TimeLine (tmlnmod, this);
    hynta = new QWidget (central);
    mnuBar = new QMenuBar(this);
    pmnu = new QMenu ("&Menu");
    repo =  Workspace::Repository::make();
    repo_view = new Workspace::RepositoryView(repo, scgr);
    repo_view->setMaximumWidth(180);
    pmnu->addAction ("Quit", this, SLOT (quit ()));
    mnuBar->addMenu (pmnu);
    this->setMenuBar (mnuBar);

    layout->addWidget (mainsplitter, 0, 0, 1, 1);
    wnd3d->setMinimumSize (300, 300);
	winsplitter->addWidget (wnd3d);
    winsplitter->addWidget (tmln);
    mainsplitter->addWidget (repo_view);
    mainsplitter->addWidget (winsplitter);
    mainsplitter->addWidget (hynta);
    for (int i = 1; i < 41; i++)
		{
			Point3d::ptr p2 = Point3d::make ();
			p2->x = i * 0.1;
			p2->y = 0;
			p2->z = 0;
			p2->r = 0;
			p2->g = 0;
			p2->b = 0;


			Camera::ptr p1 = Camera::make ();
			p1->tx = 0.05 * i;
			p1->ty = 0;
			p1->tz = 0;

			repo->addItem (p2, i, Item::FPVEC, "fpvec");
			repo->addItem (p1, i, Item::CAMERA, "camera");
		}
	for (auto it : repo->getTree ())
		{
			std::cout << "branch: " << it.first << std::endl;
			for (auto l : it.second)
				std::cout << "\t leaf: " << l.first << std::endl;
		}
    connect (tmlnmod, SIGNAL (newFrame(int)), wnd3d, SLOT(update(int)));
    tmlnmod->setMax(41);
    tmln->updateWidget();

}
void Core::quit ()
{
	qApp->exit ();
}
