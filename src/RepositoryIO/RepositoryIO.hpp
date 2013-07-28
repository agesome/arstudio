#ifndef REPOSITORY_SERIALIZER_HPP
#define REPOSITORY_SERIALIZER_HPP

#include <QFile>
#include <QString>
#include <QList>
#include <QtDebug>

#include <RepositoryNode.hpp>
#include <Bitmap.hpp>
#include <Camera.hpp>
#include <Point3D.hpp>
#include <PointCloud.hpp>

namespace arstudio {
class RepositoryIO
{
public:
  static bool serialize_to_file (const QString & path,
                                 QList<RepositoryNode *> & nodes);
  static bool deserialize_from_file (const QString & path,
                                     QList<RepositoryNode *> & nodes);
};
}
#endif
