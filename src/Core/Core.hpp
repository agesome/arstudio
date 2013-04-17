#ifndef CORE_H
#define CORE_H

#include <exception>
#include <stdexcept>

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

/**
 *      This is the core class, it implements the main application window
 *
 *      Most private members are not documented because they are only used
 * to build the UI.
 */

class Core : public QMainWindow
{
	Q_OBJECT
private:
	void init_gui (void);
	void connect_signals (void);
	void init_toolbar (void);
	void update_windows (void);

	QSplitter * h_splitter;
	QSplitter * v_splitter;
	QMenuBar  * menubar;
	QMdiArea  * mdi_area;
	QToolBar  * toolbar;

	QString last_screenshot_path;

	Repository::ptr repository_ptr;
	Scenegraph::ptr scenegraph_ptr;

	TimeLineModel    * timeline_model;
	TimeLine         * timeline;
	Window3D         * window3d;
	Window2D         * window2d;
	ProcessingDialog * processing_dialog;
	RepositoryView   * repository_view;
public:
	Core (void);
public slots:
	void menu_quit (void);
	void menu_open (void);
	void menu_about (void);
	void menu_help (void);
	void menu_settings (void);
	void processing_done (bool, const std::string &);
	void clear_repository (void);
	void make_screenshot (void);
};

#endif // CORE_H
