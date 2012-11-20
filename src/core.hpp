#ifndef CORE_H
#define CORE_H

#include <QWidget>
#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QFrame>
#include <timeline.hpp>
#include <repository_view.hpp>
#include <window3d.hpp>
#include <QSplitter>
#include <repository.hpp>
#include <camera.hpp>
#include <timelinemodel.hpp>

class Core : public QMainWindow
{
	Q_OBJECT
private:
	QWidget * central;
    QMenuBar *mnuBar;
	QMenu *pmnu;
	QGridLayout *layout;
	TimeLine *tmln;
	Workspace::Repository::ptr repo;
	RepositoryView * repo_view;
	Workspace::Scenegraph::ptr scgr;
	Window3D *wnd3d;   // window3d
	QWidget *hynta;   // right hyNta
	QPushButton *butt;
	QSplitter *mainsplitter, *winsplitter;
	Workspace::Repository *rep;
    TimeLineModel *tmlnmod;
public:
	Core();
signals:

public slots:
	void quit ();
};

#endif // CORE_H
