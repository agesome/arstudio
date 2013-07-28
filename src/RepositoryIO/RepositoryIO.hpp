#ifndef REPOSITORY_IO_HPP
#define REPOSITORY_IO_HPP

#include <QFile>
#include <QDataStream>
#include <QString>
#include <QList>
#include <QtDebug>

#include <RepositoryNode.hpp>
#include <Bitmap.hpp>
#include <Camera.hpp>
#include <Point3D.hpp>
#include <PointCloud.hpp>

namespace arstudio {
namespace io {
bool
serialize_to_file (const QString & path,
                   QList<RepositoryNode *> & nodes);
bool
deserialize_from_file (const QString & path,
                       QList<RepositoryNode *> & nodes);
}
}

#endif // REPOSITORY_IO_HPP
