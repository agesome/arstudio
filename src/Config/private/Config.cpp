#include <Config.hpp>
#include <QtDebug>

namespace arstudio {
Config::Config (QObject * parent)
  : QAbstractListModel (parent)
{
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
  if (role == PathRole)
    return m_settings.keys ().at (index.row ());
  else if (role == ValueRole)
    return m_settings.values ().at (index.row ());

  return QVariant ();
}

int
Config::rowCount (const QModelIndex &) const
{
  return m_settings.count ();
}

bool
Config::import_xml (const QString & path)
{
  QXmlStreamReader xml;
  QFile            xml_file (path);
  QStringList      prefix;

  if (!xml_file.open (QIODevice::ReadOnly | QIODevice::Text))
    return false;
  xml.setDevice (&xml_file);

  while (!xml.atEnd ())
    {
      switch (xml.readNext ())
        {
        case QXmlStreamReader::Characters:
          if (xml.isWhitespace ())
            continue;
          beginInsertRows (QModelIndex (), rowCount (), rowCount ());
          m_settings.insert (prefix.join ('.'), xml.text ().toString ());
          endInsertRows ();
          break;

        case QXmlStreamReader::StartElement:
          prefix << xml.name ().toString ();
          break;

        case QXmlStreamReader::EndElement:
          prefix.pop_back ();
          break;

        default:
          break;
        }
    }

  return !xml.hasError ();
}

void
Config::import_directory (const QString &path)
{
  QDir d (path);

  if (!d.exists ())
    return;
  for (QString subdir : d.entryList (QDir::Dirs | QDir::NoDotAndDotDot))
    import_xml (d.absolutePath () + QDir::separator () + subdir
                + QDir::separator () + "settings.xml");
}

QVariant
Config::get (const QString &path)
{
  return m_settings[path];
}

void
Config::set (int row, const QVariant & value)
{
  m_settings[m_settings.keys ().at (row)] = value;
}
}
