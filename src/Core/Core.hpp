#ifndef CORE_H
#define CORE_H

#include <QWidget>
#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QFrame>
#include <QSplitter>
#include <QMessageBox>
#include <QToolBar>
#include <QMdiArea>

#include <opencv2/highgui/highgui.hpp>

#include <TimeLineModel.hpp>
#include <TimeLine.hpp>
#include <RepositoryView.hpp>
#include <Window2D.hpp>
#include <Window3D.hpp>
#include <ProcessingDialog.hpp>

using namespace Workspace;

class Core : public QMainWindow
{
	Q_OBJECT
private:
	QMenuBar * mnuBar = new QMenuBar (this);
	QMenu *menu_file = new QMenu ("&File");
	QMenu *menu_edit = new QMenu ("&Edit");
	QMenu *menu_help = new QMenu ("&Help");

	QMdiArea *mdiArea = new QMdiArea (this);
	QSplitter *hSplitter = new QSplitter (Qt::Horizontal, this);
	QSplitter *vSplitter = new QSplitter (Qt::Vertical, this);
	QToolBar *toolbar = new QToolBar ("Toolbar", this);

	QString lastSaveLocation = QDir::currentPath () + "/untitled.png";

	Repository::ptr repo = Repository::make ();
	Scenegraph::ptr scgr = Scenegraph::make ();

	TimeLineModel *tmlnmod = new TimeLineModel (0, 0);
	TimeLine *tmln = new TimeLine (tmlnmod, this);
	Window3D *wnd3d = new Window3D (scgr, this);
	Window2D *wnd2d = new Window2D (scgr, this);
	ProcessingDialog * processing = new ProcessingDialog (this);
	RepositoryView * repo_view = new Workspace::RepositoryView (repo, scgr);

	void initGUI ();
	void connectSignals ();
	void initToolbar ();
	void updateWindows ();
public:
	Core();
public slots:
	void quit ();
	void open ();
	void about ();
	void help ();
	void settings ();
	void processingDone (bool, std::string);
	void clearRepository (void);
	void makeScreenshot (void);
};
#endif // CORE_H
