#ifndef CONFIG_VIEW_H
#define CONFIG_VIEW_H

#include <QMainWindow>
#include <QWidget>
#include <QTreeView>
#include <QHeaderView>
#include <map>
#include <config.hpp>


class ConfigView : public QTreeView
{
	Q_OBJECT
public:
	ConfigView (QWidget * MainWindow = nullptr);
public slots:
	void itemChangedHandler (QStandardItem *);
};

#endif // CONFIG_VIEW_H