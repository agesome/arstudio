#ifndef CORE_H
#define CORE_H

#include <QWidget>
#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QFrame>
#include <QSplitter>
#include <QMessageBox>

#include <opencv2/highgui/highgui.hpp>
#include <fstream>

#include <ui/timeline.hpp>
#include <ui/repository_view.hpp>
#include <ui/window3d.hpp>
#include <ui/timelinemodel.hpp>
#include <ui/processing_dialog.hpp>

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
	QSplitter *mainsplitter = new QSplitter (Qt:: Horizontal, central);
	QSplitter *winsplitter = new QSplitter (Qt:: Vertical, central);
	QPushButton *open_processing = new QPushButton ("Process");

	Workspace::Repository::ptr repo = Workspace::Repository::make ();
	Workspace::Scenegraph::ptr scgr = Workspace::Scenegraph::make ();

	TimeLineModel *tmlnmod = new TimeLineModel (1, 10);
	TimeLine *tmln = new TimeLine (tmlnmod, this);
	Window3D *wnd3d = new Window3D (scgr, central);
	ProcessingDialog * processing = new ProcessingDialog (this);
	RepositoryView * repo_view = new Workspace::RepositoryView (repo, scgr);

	void initGUI ();
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
};
#endif // CORE_H
