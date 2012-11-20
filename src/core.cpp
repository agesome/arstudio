#include <core.hpp>

Core::Core() :
	QMainWindow ()
{
	central = new QWidget (this);
	this->setObjectName (QString::fromUtf8 ("Core"));
	this->setGeometry (10, 10, 800, 500);
	this->setCentralWidget (central);
<<<<<<< HEAD
    layout = new QGridLayout (central);
    mainsplitter = new QSplitter (Qt:: Horizontal, central);
    winsplitter = new QSplitter (Qt:: Vertical, central);
    scgr_sel = new ScenegraphSelector (central);
    scgr = Workspace::Scenegraph::make (scgr_sel);
    wnd3d = new Window3D (scgr, central);
    tmlnmod = new TimeLineModel(1, 10);
    tmln = new TimeLine (tmlnmod, this);
    hynta = new QWidget (central);
    rep = new Workspace::Repository ();
    mnuBar = new QMenuBar(this);
    pmnu = new QMenu ("&Menu");

    pmnu->addAction ("Quit", this, SLOT (quit ()));
    mnuBar->addMenu (pmnu);
    this->setMenuBar (mnuBar);

    layout->addWidget (mainsplitter, 0, 0, 1, 1);
    scgr_sel->addCheckbox (Item::CAMERA);
	scgr_sel->addCheckbox (Item::FPVEC);
    wnd3d->setMinimumSize (300, 300);
	winsplitter->addWidget (wnd3d);
    winsplitter->addWidget (tmln);
    mainsplitter->addWidget (scgr_sel);
    mainsplitter->addWidget (winsplitter);
    mainsplitter->addWidget (hynta);
    for (int i = 1; i < 41; i++)
=======

	pmnu = new QMenu ("&Menu");
	mnuBar.addMenu (pmnu);
	pmnu->addAction ("Quit", this, SLOT (quit ()));
	this->setMenuBar (&mnuBar);

	layout = new QGridLayout (central);
	mainsplitter = new QSplitter (Qt:: Horizontal, central);
	winsplitter = new QSplitter (Qt:: Vertical, central);
	layout->addWidget (mainsplitter, 0, 0, 1, 1);

	scgr = Workspace::Scenegraph::make ();
	repo = Workspace::Repository::make ();
	repo_view = new Workspace::RepositoryView (repo, scgr);
	mainsplitter->addWidget (repo_view);

	wnd3d = new Window3D (scgr, central);
	wnd3d->setMinimumSize (300, 300);
	winsplitter->addWidget (wnd3d);

	tmln = new TimeLine (10, this);
	winsplitter->addWidget (tmln);
	mainsplitter->addWidget (winsplitter);

	hynta = new QWidget (central);
	mainsplitter->addWidget (hynta);

	for (int i = 1; i < 42; i++)
>>>>>>> 86866e2bc19b9b471850248534400c8101f4ded7
		{
			Point3d::ptr p2 = Point3d::make ();
			p2->x = i * 0.1;
			p2->y = 0;
			p2->z = 0;
			p2->r = 0;
			p2->g = 0;
			p2->b = 0;

<<<<<<< HEAD
			boost::shared_ptr<Camera> p1 = boost::make_shared<Camera>();;
            p1->tx = 0.1 * i;
=======
			Camera::ptr p1 = Camera::make ();
			p1->tx = 0.05 * i;
>>>>>>> 86866e2bc19b9b471850248534400c8101f4ded7
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
<<<<<<< HEAD
	for (auto it : rep->getSequences ())
		scgr->addSequence (it.second);
    connect (tmlnmod, SIGNAL (newFrame(int)), wnd3d, SLOT(update(int)));
    tmlnmod->setMax(41);
    tmln->updateWidget();
=======
	tmln->setMax (41);
	connect (tmln, SIGNAL (nextFrame (int)), wnd3d, SLOT (update (int)));
	connect (wnd3d, SIGNAL (drawed ()), tmln, SLOT (onDrawed ()));
>>>>>>> 86866e2bc19b9b471850248534400c8101f4ded7
}
void Core::quit ()
{
	qApp->exit ();
}
