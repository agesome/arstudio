#ifndef CONFIG_VIEW_H
#define CONFIG_VIEW_H

#include <QMainWindow>
#include <QWidget>
#include <QTreeWidgetItem>
#include <map>
#include <config.hpp>

class ConfigView : public QTreeWidget
{
	Q_OBJECT
public:
	ConfigView (QWidget * MainWindow = nullptr);
	void populate (void);
private:
};

#endif // CONFIG_VIEW_H