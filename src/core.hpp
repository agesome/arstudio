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
<<<<<<< HEAD
#include <timelinemodel.hpp>
=======

>>>>>>> 86866e2bc19b9b471850248534400c8101f4ded7
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
<<<<<<< HEAD
	Workspace::Repository *rep;
    TimeLineModel *tmlnmod;
=======
>>>>>>> 86866e2bc19b9b471850248534400c8101f4ded7
public:
	Core();
signals:

public slots:
	void quit ();
};

#endif // CORE_H
