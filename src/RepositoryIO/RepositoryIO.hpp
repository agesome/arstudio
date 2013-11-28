#ifndef REPOSITORY_IO_HPP
#define REPOSITORY_IO_HPP

#include <QFile>
#include <QDataStream>
#include <QString>
#include <QList>
#include <QtDebug>

#include <Bitmap.hpp>
#include <Camera.hpp>
#include <Point3D.hpp>
#include <PointCloud.hpp>

namespace arstudio {
namespace io {
bool
serialize_to_file (const QString & path,
                   QList<Sequence::ptr> & nodes);
bool
deserialize_from_file (const QString & path,
                       QList<Sequence::ptr> &nodes);
}
}

#endif // REPOSITORY_IO_HPP
