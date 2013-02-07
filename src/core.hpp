#ifndef CORE_H
#define CORE_H

#include <QWidget>
#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QFrame>
#include <QSplitter>

#include <opencv2/highgui/highgui.hpp>
#include <fstream>

#include <timeline.hpp>
#include <repository_view.hpp>
#include <window3d.hpp>
#include <timelinemodel.hpp>
#include <algo_pipeline.hpp>
#include <logger.hpp>
#include <config.hpp>
#include <config_view.hpp>
#include <video.hpp>

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
	QSplitter *rsplitter = new QSplitter (Qt:: Vertical, central);

	TimeLineModel *tmlnmod = new TimeLineModel (1, 10);
	TimeLine *tmln = new TimeLine (tmlnmod, this);
	Workspace::Repository::ptr repo = Workspace::Repository::make ();
	Workspace::Scenegraph::ptr scgr = Workspace::Scenegraph::make ();
	RepositoryView * repo_view = new Workspace::RepositoryView (repo, scgr);
	Window3D *wnd3d = new Window3D (scgr, central);
	// ConfigView *cview = new ConfigView (central);
	AlgoPipeline * apipe = new AlgoPipeline ();
	// Config * config = new Config ();
	Video *vselect = new Video (NULL, apipe);

	QPushButton *reload_button = new QPushButton ("Reload all settings");

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
	void updateTimeline (void);
	void callSettingsReload (void);
};
#endif // CORE_H
