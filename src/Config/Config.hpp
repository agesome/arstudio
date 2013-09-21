#ifndef CONFIG_H
#define CONFIG_H

#include <QXmlStreamReader>
#include <QSharedPointer>
#include <QAbstractListModel>
#include <QMap>
#include <QFile>
#include <QDir>
#include <QStringList>

namespace arstudio {
/**
 * @brief This class handles runtime settings for algorithms
 *
 * This class handles loading and storage of per-algorithm settings in a simple
 * list-like form. It also implements QAbstractListModel to serve as a model
 * to display all stored settings to the user for editing.
 */
class Config : public QAbstractListModel
{
  Q_OBJECT
public:
  typedef QSharedPointer <Config> ptr;
  enum ConfigRoles
  {
    PathRole = Qt::UserRole + 1,
    ValueRole
  };

  Config (QObject * parent = nullptr);

  static ptr
  make (QObject * parent = nullptr);

  QVariant data (const QModelIndex & index, int role = PathRole) const;
  int rowCount (const QModelIndex & parent = QModelIndex ()) const;

  /**
   * @brief Import settings from a single XML file
   * @param path location of the XML file
   * @return true on success, false otherwise
   */
  bool import_xml (const QString & path);

  /**
   * @brief Import settings from every subdirectory
   * @param path path to the parent directory
   * By convention, this method looks for a file named "settings.xml" in every
   * subdirectory and calls import_xml on that file
   */
  void import_directory (const QString & path);

  /**
   * @brief Get a setting at specified path
   * @param path path of the setting
   * @return value of the setting
   */
  QVariant get (const QString & path);

  /**
   * @brief Assign a value to setting at row in internal settings map
   *
   * This method is called from QML TableView when user edits a setting
   */
  Q_INVOKABLE void set (int row, const QVariant & value);

  /**
   * @brief set a setting, to be used internally
   * @param key the key to assign
   * @param value desired value
   */
  void set (const QString & key, const QVariant & value);

protected:
  QHash<int, QByteArray> roleNames () const;
private:
  /**
   * @brief Internal setting map
   *
   * Constant element positions are guaranteed because all settings are
   * imported at program start
   */
  QMap<QString, QVariant> m_settings;
};
}

#endif // CONFIG_H
