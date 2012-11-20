#ifndef REPOSITORY_VIEW_H
#define REPOSITORY_VIEW_H

#include <QMainWindow>
#include <QWidget>
#include <QStandardItemModel>
#include <QTreeView>
#include <string>
#include <repository.hpp>
#include <scenegraph.hpp>

namespace Workspace
{
class RepositoryView : public QTreeView
{
	Q_OBJECT
public:
	RepositoryView (Repository::ptr, Scenegraph::ptr, QWidget * MainWindow = nullptr);

private:
	QStandardItemModel * model;
	std::map <std::string, int> child_count;
	Scenegraph::ptr scgr;
	Repository::ptr repo;

signals:

private slots:
	void onItemChanged (QStandardItem *);
};
}
#endif // REPOSITORY_VIEW_H
