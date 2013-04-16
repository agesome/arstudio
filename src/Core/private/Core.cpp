#include <Core.hpp>

Core::Core() : QMainWindow ()
{
	initGUI ();
	connectSignals ();
	initToolbar ();
	Logger::setRepository (repo);
}

/**
        This slot makes use of QGLWidget::renderPixmap () to make a screenshot of the 3D window.
 */

void Core::makeScreenshot (void)
{
	QPixmap p = wnd3d->renderPixmap ();

	QString fileName = QFileDialog::getSaveFileName (this, "Save Screenshot",
	                                                 lastSaveLocation, "PNG Image (*.png)");

	if (fileName.isNull ())
		return;
	lastSaveLocation = fileName;
	p.save (fileName);
}

/**
        This slot is called to clear the Repository when a new file is to be processed.
 */

void Core::clearRepository (void)
{
	Logger::getInstance ().resetFrameCounter ();

	repo->Clear ();
	updateWindows ();
}

/**
        This slot is called when processing stops, either because it has completed,
        or because there was an error.

        \param success indicates whether the processing was successful
        \param e contains the error message in case of processing failure
 */

void Core::processingDone (bool success, std::string e)
{
	if (success)
		{
			updateWindows ();
		}
	else
		{
			QMessageBox msg;
			msg.setText (QString ("Processing failed: ") + QString::fromStdString (e));
			msg.exec ();
		}
}

/**
        This method is called to update the scenegraph and make all the windows
        display the first frame
 */

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
	this->setCentralWidget (vSplitter);

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

	hSplitter->addWidget (repo_view);
	hSplitter->addWidget (mdiArea);
	vSplitter->addWidget (hSplitter);
	vSplitter->addWidget (tmln);

	repo_view->setMaximumWidth (repo_view->sizeHint ().width ());
	tmln->setMaximumHeight (tmln->sizeHint ().height ());

	mdiArea->addSubWindow (wnd3d);
	mdiArea->addSubWindow (wnd2d);

	wnd3d->setMinimumSize (300, 300);
	wnd2d->setMinimumSize (300, 300);

	this->resize (this->sizeHint ());

	processing->setWindowFlags (Qt::Window);
}

void Core::initToolbar ()
{
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
}

void Core::connectSignals ()
{
	qRegisterMetaType<std::string>("std::string");

	connect (tmlnmod, SIGNAL (newFrame (int)), wnd3d, SLOT (update (int)));
	connect (tmlnmod, SIGNAL (newFrame (int)), wnd2d, SLOT (update (int)));
	connect (processing, SIGNAL (done_processing (bool, std::string)), this,
	         SLOT (processingDone (bool, std::string)));
	connect (processing, SIGNAL (clearRepository ()), this, SLOT (clearRepository ()));

	connect (repo_view, SIGNAL (selectionChanged ()), wnd3d, SLOT (update ()));
	connect (repo_view, SIGNAL (selectionChanged ()), wnd2d, SLOT (update ()));
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
