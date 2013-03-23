#include <ui/core.hpp>

Core::Core() :
	QMainWindow ()
{
	initGUI ();
	connect (tmlnmod, SIGNAL (newFrame (int)), wnd3d, SLOT (update (int)));
	connect (tmlnmod, SIGNAL (newFrame (int)), wnd2d, SLOT (update (int)));
	connect (processing, SIGNAL (done_processing (bool, std::string)), this,
	         SLOT (processingDone (bool, std::string)));
	connect (processing, SIGNAL (clearRepository ()), this, SLOT (clearRepository ()));

	connect (repo_view, SIGNAL (selectionChanged ()), wnd3d, SLOT (update ()));
	connect (repo_view, SIGNAL (selectionChanged ()), wnd2d, SLOT (update ()));

	Logger::setRepository (repo);
}

void Core::makeScreenshot (void)
{
	QPixmap p = QPixmap::grabWidget (wnd3d);

	QString fileName = QFileDialog::getSaveFileName (this,
	                                                 tr ("Save Screenshot"), lastSaveLocation, tr ("PNG Image (*.png)"));

	if (fileName.isNull ())
		return;
	lastSaveLocation = QFileInfo (fileName).absolutePath ();
	p.save (fileName);
}

void Core::clearRepository (void)
{
	Logger::getInstance ().resetFrameCounter ();

	repo->Clear ();
	updateWindows ();
}

void Core::processingDone (bool success, std::string e)
{
	if (success)
		updateWindows ();
	else
		{
			QMessageBox msg;
			msg.setText (QString ("Processing failed: ") + QString::fromStdString (e));
			msg.exec ();
		}
}

void Core::updateWindows ()
{
	tmlnmod->setMin (1);
	tmlnmod->setMax (scgr->getMaxFrame ());
	tmln->updateWidget ();
	wnd3d->update (1);
	wnd2d->update (1);
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
	wnd2d->setMinimumSize (300, 300);
	// have nothing to show yet
	wnd2d->hide ();

	mainsplitter->addWidget (repo_view);
	mainsplitter->addWidget (rightsplitter);

	winsplitter->addWidget (wnd3d);
	winsplitter->addWidget (wnd2d);
	rightsplitter->addWidget (winsplitter);
	rightsplitter->addWidget (tmln);

	processing->setWindowFlags (Qt::Window);

	QAction * screenshot = new QAction (QIcon::fromTheme ("image-x-generic"),
	                                    "Screenshot", toolbar);
	connect (screenshot, SIGNAL (triggered ()), this, SLOT (makeScreenshot ()));
	toolbar->addAction (screenshot);

	QAction * clear = new QAction (QIcon::fromTheme ("edit-clear"),
	                               "Clear Repository", toolbar);
	connect (clear, SIGNAL (triggered ()), this, SLOT (clearRepository ()));
	toolbar->addAction (clear);

	QAction * process = new QAction (QIcon::fromTheme ("system-run"),
	                                 "Process", toolbar);
	connect (process, SIGNAL (triggered ()), processing, SLOT (show ()));
	toolbar->addAction (process);

	toolbar->setToolButtonStyle (Qt::ToolButtonTextUnderIcon);
	this->addToolBar (toolbar);

	qRegisterMetaType<std::string>("std::string");
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
