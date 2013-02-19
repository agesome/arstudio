#include <ui/core.hpp>

Core::Core() :
	QMainWindow ()
{
	initGUI ();
	connect (tmlnmod, SIGNAL (newFrame (int)), wnd3d, SLOT (update (int)));
	connect (open_processing, SIGNAL (clicked ()), processing, SLOT (show ()));
	connect (processing, SIGNAL (done_processing ()), this, SLOT (processingDone ()));

	Logger::setRepository (repo);
}

void Core::processingDone (void)
{
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
	winsplitter->addWidget (open_processing);
	mainsplitter->addWidget (repo_view);
	mainsplitter->addWidget (winsplitter);

	processing->setWindowFlags (Qt::Window);
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
