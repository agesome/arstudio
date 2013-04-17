#ifndef CONFIG_EDITOR_H
#define CONFIG_EDITOR_H

#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QStringList>

#include <Config.hpp>

/**
 *      This class implements a widget to display and edit settings from
 * Config
 */
class ConfigEditor : public QTreeWidget
{
  Q_OBJECT
public:
  explicit
  ConfigEditor (Config::ptr, QWidget * parent = nullptr);
  void config_callback (const std::string &, const std::string &);
private:
  Config::ptr config;             // < pointer to an instance of Config to
                                  // be
                                  // edited
private slots:
  void item_changed_handler (QTreeWidgetItem *, int);
};

#endif // CONFIG_EDITOR_H
