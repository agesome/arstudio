#include <Core.hpp>

Core::Core() : QMainWindow ()
{
	init_gui ();
	connect_signals ();
	init_toolbar ();
	Logger::setRepository (repository_ptr);
}

/**
        This slot makes use of QGLWidget::renderPixmap () to make a screenshot of the 3D window.
 */

void
Core::make_screenshot (void)
{
	QPixmap p = window3d->renderPixmap ();

	QString filename = QFileDialog::getSaveFileName (this, "Save Screenshot",
	                                                 last_screenshot_path, "PNG Image (*.png)");

	if (filename.isNull ())
		{
			return;
		}
	last_screenshot_path = filename;
	p.save (filename);
}

/**
        This slot is called to clear the Repository when a new file is to be processed.
 */

void
Core::clear_repository (void)
{
	Logger::getInstance ().resetFrameCounter ();

	repository_ptr->Clear ();
	update_windows ();
}

/**
        This slot is called when processing stops, either because it has completed,
        or because there was an error.

        \param success indicates whether the processing was successful
        \param e contains the error message in case of processing failure
 */

void
Core::processing_done (bool success, const std::string & error_msg)
{
	if (success)
		{
			update_windows ();
		}
	else
		{
			QMessageBox msg;
			QString message = QString ("Processing failed: ")
			                  + QString::fromStdString (error_msg);
			msg.setText (message);
			msg.exec ();
		}
}

/**
        This method is called to update the scenegraph and make all the windows
        display the first frame
 */

void
Core::update_windows ()
{
	timeline_model->setMin (1);
	timeline_model->setMax (scenegraph_ptr->getMaxFrame ());
	timeline->updateWidget ();
	window3d->update (1);
	window2d->update (1);
}

void
Core::init_gui ()
{
	QMenu * menu_file;
	QMenu * menu_edit;
	QMenu * menu_help;

	h_splitter = new QSplitter (Qt::Horizontal, this);
	v_splitter = new QSplitter (Qt::Vertical, this);
	menubar = new QMenuBar (this);
	menu_file = new QMenu ("&File");
	menu_edit = new QMenu ("&Edit");
	menu_help = new QMenu ("&Help");
	mdi_area = new QMdiArea (this);
	toolbar = new QToolBar ("Toolbar", this);

	last_screenshot_path = QDir::currentPath () + "/untitled.png";

	repository_ptr = Repository::make ();
	scenegraph_ptr = Scenegraph::make ();

	timeline_model = new TimeLineModel (0, 0);
	timeline = new TimeLine (timeline_model, this);
	window3d = new Window3D (scenegraph_ptr, this);
	window2d = new Window2D (scenegraph_ptr, this);
	processing_dialog = new ProcessingDialog (this);
	repository_view = new RepositoryView (repository_ptr, scenegraph_ptr);

	this->setCentralWidget (v_splitter);

	menu_file->addAction ("Open", this, SLOT (menu_open ()));
	menu_file->addSeparator ();
	menu_file->addAction ("Exit", this, SLOT (menu_quit ()));
	menu_edit->addAction ("Settings", this, SLOT (menu_settings ()));
	menu_help->addAction ("Help", this, SLOT (menu_help ()), Qt::Key_F1);
	menu_help->addSeparator ();
	menu_help->addAction ("About", this, SLOT (menu_about ()));
	menubar->addMenu (menu_file);
	menubar->addMenu (menu_edit);
	menubar->addMenu (menu_help);
	this->setMenuBar (menubar);

	h_splitter->addWidget (repository_view);
	h_splitter->addWidget (mdi_area);
	v_splitter->addWidget (h_splitter);
	v_splitter->addWidget (timeline);

	repository_view->setMaximumWidth (repository_view->sizeHint ().width ());
	timeline->setMaximumHeight (timeline->sizeHint ().height ());

	mdi_area->addSubWindow (window3d);
	mdi_area->addSubWindow (window2d);

	window3d->setMinimumSize (300, 300);
	window2d->setMinimumSize (300, 300);

	this->resize (this->sizeHint ());

	processing_dialog->setWindowFlags (Qt::Tool);
}

void Core::init_toolbar ()
{
	QAction * screenshot = new QAction (QIcon::fromTheme ("image-x-generic"),
	                                    "Screenshot", toolbar);

	connect (screenshot, SIGNAL (triggered ()), this, SLOT (make_screenshot ()));
	toolbar->addAction (screenshot);

	QAction * clear = new QAction (QIcon::fromTheme ("edit-clear"),
	                               "Clear Repository", toolbar);
	connect (clear, SIGNAL (triggered ()), this, SLOT (clear_repository ()));
	toolbar->addAction (clear);

	QAction * process = new QAction (QIcon::fromTheme ("system-run"),
	                                 "Process", toolbar);
	connect (process, SIGNAL (triggered ()), processing_dialog, SLOT (show ()));
	toolbar->addAction (process);

	toolbar->setToolButtonStyle (Qt::ToolButtonTextUnderIcon);
	this->addToolBar (toolbar);
}

void Core::connect_signals ()
{
	qRegisterMetaType<std::string>("std::string");

	connect (timeline_model, SIGNAL (newFrame (int)), window3d,
	         SLOT (update (int)));
	connect (timeline_model, SIGNAL (newFrame (int)), window2d,
	         SLOT (update (int)));
	connect (processing_dialog, SIGNAL (processing_done (bool, const std::string &)),
	         this, SLOT (processing_done (bool, const std::string &)));
	connect (processing_dialog, SIGNAL (clearRepository ()), this,
	         SLOT (clear_repository ()));
	connect (repository_view, SIGNAL (selectionChanged ()), window3d,
	         SLOT (update ()));
	connect (repository_view, SIGNAL (selectionChanged ()), window2d,
	         SLOT (update ()));
}

void
Core::menu_open (void)
{
	qDebug () << "Open file";
}

void
Core::menu_about (void)
{
	qDebug () << "About";
}

void
Core::menu_settings (void)
{
	qDebug () << "Settings";
}

void
Core::menu_help (void)
{
	qDebug () << "Help";
}

void
Core::menu_quit (void)
{
	qApp->exit ();
}
