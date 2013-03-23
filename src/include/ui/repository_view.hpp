#ifndef REPOSITORY_VIEW_H
#define REPOSITORY_VIEW_H

#include <QMainWindow>
#include <QWidget>
#include <QTreeWidgetItem>
#include <string>
#include <repository.hpp>
#include <scenegraph.hpp>
#include <QDebug>

namespace Workspace
{
class RepositoryView : public QTreeWidget
{
	Q_OBJECT
public:
	RepositoryView (Repository::ptr, Scenegraph::ptr, QWidget * MainWindow = nullptr);

private:
	Scenegraph::ptr scgr;
	Repository::ptr repo;

signals:
	void selectionChanged (void);

private slots:
	void onItemChanged (QTreeWidgetItem * item, int col);
};
}
#endif // REPOSITORY_VIEW_H
