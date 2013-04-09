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

#include <opencv2/highgui/highgui.hpp>

#include <TimeLineModel.hpp>
#include <TimeLine.hpp>
#include <RepositoryView.hpp>
#include <Window2D.hpp>
#include <Window3D.hpp>
#include <ProcessingDialog.hpp>

class Core : public QMainWindow
{
	Q_OBJECT
private:
	QWidget * central = new QWidget (this);
	QMenuBar *mnuBar = new QMenuBar (this);
	QMenu *menu_file = new QMenu ("&File");
	QMenu *menu_edit = new QMenu ("&Edit");
	QMenu *menu_help = new QMenu ("&Help");
	QGridLayout *layout = new QGridLayout (central);
	QPushButton *butt;
	QSplitter *mainsplitter = new QSplitter (Qt::Horizontal, central);
	QSplitter *rightsplitter = new QSplitter (Qt::Vertical, central);
	QSplitter *winsplitter = new QSplitter (Qt::Horizontal, central);
	QToolBar *toolbar = new QToolBar ("Toolbar", this);

	QString lastSaveLocation = QDir::currentPath () + "/untitled.png";

	Workspace::Repository::ptr repo = Workspace::Repository::make ();
	Workspace::Scenegraph::ptr scgr = Workspace::Scenegraph::make ();

	TimeLineModel *tmlnmod = new TimeLineModel (0, 0);
	TimeLine *tmln = new TimeLine (tmlnmod, this);
	Window3D *wnd3d = new Window3D (scgr, central);
	Window2D *wnd2d = new Window2D (scgr, central);
	ProcessingDialog * processing = new ProcessingDialog (this);
	RepositoryView * repo_view = new Workspace::RepositoryView (repo, scgr);

	void initGUI ();
	void updateWindows ();
public:
	Core();
signals:

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
