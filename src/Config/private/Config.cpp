#include <Config.hpp>
#include <QtDebug>
#include <QSettings>

namespace arstudio {
Config::Config (QObject * parent)
  : QAbstractListModel (parent)
{
}

Config::~Config()
{
  QStringList list = m_settings.keys();
  for(int i=0; i< m_settings.size(); i++)
  {
    delete m_settings[list.at(i)];
  }
}

Config::ptr
Config::make (QObject * parent)
{
  return ptr (new Config (parent));
}

QHash<int, QByteArray>
Config::roleNames () const
{
  QHash<int, QByteArray> roles;
  roles[PathRole]  = "path";
  roles[ValueRole] = "value";
  return roles;
}

QVariant
Config::data (const QModelIndex & index, int role) const
{
  const QString key_name = m_settings[m_current_algo]->allKeys ().at (index.row());

  switch (role)
    {
    case PathRole:
      return key_name;
    case ValueRole:
      return m_settings[m_current_algo]->value (key_name);
    default:
      break;
    }

  return QVariant ();
}

int
Config::rowCount (const QModelIndex &) const
{
  return m_settings[m_current_algo]->allKeys ().count ();
}

QStringList
Config::get_algo_list ()
{
  QStringList list = m_settings.keys ();
  return list.toSet ().toList ();
}

bool
Config::import_ini (const QString & path, const QString & algo)
{
  QFile ini_file (path);

  if (!ini_file.open (QIODevice::ReadOnly | QIODevice::Text))
    return false;
  QSettings * temp = new QSettings (ini_file.fileName (), QSettings::IniFormat);
  m_settings.insert (algo, temp);
  m_current_algo = algo;
  return true;
}


void
Config::import_directory (const QString &path)
{
  m_algo_folder = QDir(path);
  QDir d (path);

  if (!d.exists ())
    return;
  for (QString subdir : d.entryList (QDir::Dirs | QDir::NoDotAndDotDot))
    import_ini (d.absolutePath () + QDir::separator () + subdir
                + QDir::separator () + "settings.ini", subdir);
}

QVariant
Config::get (const QString & algo, const QString &path)
{
  //crutch to avoid internal algorithm in tableview
  if(algo == "internal")
    {
      return m_internal_settings[path];
    }

  return m_settings[algo]->value (path);
}

void
Config::set (int row, const QVariant & value)
{
  m_settings[m_current_algo]->setValue(m_settings[m_current_algo]->allKeys ().at (row), value);
}

void
Config::set (const QString & algo, const QString & key, const QVariant &value)
{
  bool is_new = !m_settings.contains (algo);

  //crutch to avoid internal algorithm in tableview
  if(algo == "internal")
    {
      m_internal_settings[key] = value;
      return;
    }
  if (is_new)
    {
      beginInsertRows (QModelIndex (), rowCount (), rowCount ());
      QSettings *temp = new QSettings ();
      m_settings.insert (algo, temp);
    }
  m_settings[algo]->setValue (key, value);
  if (is_new)
    endInsertRows ();
}

void
Config::set_algo (const QString & algo)
{
  beginResetModel ();
  m_current_algo = algo;
  endResetModel ();
}

void
Config::load_defaults ()
{
  //QSettings works directly with ini-file so any changes made to QSettings will be saved to default.ini
  beginResetModel ();
  QFile file (m_algo_folder.absolutePath () + QDir::separator () + m_current_algo + QDir::separator () + "default.ini");
  QSettings *temp = new QSettings (file.fileName (), QSettings::IniFormat);
  QStringList list = m_settings[m_current_algo]->allKeys ();
  int count = m_settings[m_current_algo]->allKeys ().count ();
  for (int i = 0; i < count; i++)
    {
      m_settings[m_current_algo]->setValue (list.at (i), temp->value (list.at (i)));
    }
  endResetModel ();
  delete temp;
}

}
