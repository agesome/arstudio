#include <RepositoryIO.hpp>

namespace arstudio {
bool
RepositoryIO::serialize_to_file (const QString & path,
                                 QList<RepositoryNode *> & nodes)
{
  QFile output (path);

  if (!output.open (QFile::WriteOnly))
    return false;

#ifndef QT_NO_DEBUG
  qDebug ("Serializing a list of %d nodes", nodes.count ());
#endif

  ap::Repository serialized;
  ap::Node     * node;

  for (RepositoryNode * rn : nodes)
    {
      node = serialized.add_nodes ();
      node->set_name (rn->name ().toStdString ());

      /*
       * Complicated. Here we need to both know the type of items in the
       * sequence to cast down, and the type of protobuf extension id, to
       * add corresponding extension to the item; the extension id is
       * template magic, so we use C++11 'decltype' keyword to query it's type.
       */

      switch (rn->type ())
        {
        case ap::Node::Bitmap:
          node->set_type (ap::Node::Bitmap);
          append_items<Bitmap, decltype (ap::Bitmap::bitmap)>
            (rn->ptr (), node, ap::Bitmap::bitmap);
          break;

        case ap::Node::Camera:
          node->set_type (ap::Node::Camera);
          append_items<Camera, decltype (ap::Camera::camera)>
            (rn->ptr (), node, ap::Camera::camera);
          break;

        case ap::Node::Point:
          node->set_type (ap::Node::Point);
          append_items<Point3D, decltype (ap::Point::point)>
            (rn->ptr (), node, ap::Point::point);
          break;

        case ap::Node::PointCloud:
          node->set_type (ap::Node::PointCloud);
          append_items<PointCloud, decltype (ap::PointCloud::point_cloud)>
            (rn->ptr (), node, ap::PointCloud::point_cloud);
          break;

        default: break;
        }
    }

  serialized.SerializeToFileDescriptor (output.handle ());

#ifndef QT_NO_DEBUG
  qDebug ("Success. File size %lld bytes", output.size ());
#endif

  output.close ();
  return true;
}

bool
RepositoryIO::deserialize_from_file (const QString &path,
                                     QList<RepositoryNode *> &nodes)
{
  QFile input (path);
  bool  success;

  if (!input.open (QFile::ReadOnly))
    return false;

  ap::Repository repository;
  success = repository.ParseFromFileDescriptor (input.handle ());
#ifndef QT_NO_DEBUG
  if (success)
    qDebug ("Successfully parsed Repository with %d nodes from %s",
            repository.nodes_size (), path.toLocal8Bit ().data ());
  else
    qDebug ("Failed to parse %s", path.toLocal8Bit ().data ());
#endif
  if (!success)
    return false;

  RepositoryNode * node;
  Sequence::ptr    s;

  for (int i = 0; i < repository.nodes_size (); i++)
    {
      const ap::Node & n = repository.nodes (i);
      switch (n.type ())
        {
        case ap::Node::Bitmap:
          s = node_to_sequence<Bitmap> (n);
          break;

        case ap::Node::Camera:
          s = node_to_sequence<Camera> (n);
          break;

        case ap::Node::Point:
          s = node_to_sequence<Point3D> (n);
          break;

        case ap::Node::PointCloud:
          s = node_to_sequence<PointCloud> (n);
          break;

        default: break;
        }
      node = new RepositoryNode (QString::fromStdString (n.name ()), s);
      nodes.push_back (node);
    }

  return true;
}
}
