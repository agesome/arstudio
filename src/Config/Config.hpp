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
 * Settings storage class
 *
 * This class handles loading and storage of per-algorithm settings in a
 * simple
 * list-like form. It also implements QAbstractListModel to serve as a
 * model
 * to display all stored settings to the user for editing.
 *
 * Example of XML settings file:
 * ~~~~~{.xml}
 *     <algo_name>
 *       <setting1>text</setting1>
 *       <setting2>42</setting2>
 *       <subpath>
 *         <setting1>1</setting1>
 *       </subpath>
 *     </algo_name>
 * ~~~~~
 * The above gets parsed to the following setting list (path->value)
 * ~~~~
 *     algo_name.setting1         -> text
 *     algo_name.setting2         -> 42
 *     algo_name.subpath.setting1 -> 1
 * ~~~~
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
  make (QObject * parent = nullptr)
  {
    return ptr (new Config (parent));
  }

  QVariant data (const QModelIndex & index, int role = PathRole) const;
  int rowCount (const QModelIndex & parent = QModelIndex ()) const;

  /**
   * Import settings from a single XML file
   * @param path path to the file
   */
  bool import_xml (const QString & path);
  /**
   * Import settings from all subdirectories of a directory
   *
   * By convention, this method looks for a file names "settings.xml" in
   * every
   * subdirectory and imports that file
   * @param path path to the directory
   */
  void import_directory (const QString & path);

  /**
   * Get a setting with specified path
   * @param path path to the setting
   */
  QVariant get (const QString & path);
  Q_INVOKABLE void set (int, const QVariant &);
protected:
  QHash<int, QByteArray> roleNames () const;
private:
  QMap<QString, QVariant> m_settings;
};
}

#endif // CONFIG_H
