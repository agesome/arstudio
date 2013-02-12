#ifndef CONFIG_EDITOR_H
#define CONFIG_EDITOR_H

#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QStringList>

#include <config.hpp>

#include <iostream>

class ConfigEditor : public QTreeWidget
{
	Q_OBJECT;
public:
	explicit ConfigEditor (Config::ptr, QWidget * parent = nullptr);
	void configCallback (std::string, std::string);
private:
	Config::ptr config;
private slots:
	void itemChangedHandler (QTreeWidgetItem *, int);
};
#endif // CONFIG_EDITOR_H
