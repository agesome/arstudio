#ifndef CONFIG_VIEW_H
#define CONFIG_VIEW_H

#include <QMainWindow>
#include <QWidget>
#include <QTreeView>
#include <QHeaderView>

class ConfigView : public QTreeView
{
	Q_OBJECT
public:
	ConfigView (QWidget * MainWindow = nullptr);
};
#endif // CONFIG_VIEW_H
