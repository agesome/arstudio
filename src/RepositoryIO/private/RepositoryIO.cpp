#include <RepositoryIO.hpp>

namespace arstudio {
bool
RepositoryIO::serialize_to_file (const QString & path,
                                 QList<RepositoryNode *> & nodes)
{
  QFile output (path);

  if (!output.open (QFile::WriteOnly))
    return false;

  qDebug ("Serializing a list of %d nodes", nodes.count ());

  qDebug ("Success. File size %lld bytes", output.size ());

  output.close ();
  return true;
}

bool
RepositoryIO::deserialize_from_file (const QString &path,
                                     QList<RepositoryNode *> &nodes)
{
  QFile input (path);
}
}
