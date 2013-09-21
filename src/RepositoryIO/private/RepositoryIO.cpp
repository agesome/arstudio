#include <RepositoryIO.hpp>

namespace arstudio {
namespace io {
constexpr int FILE_MAGIC   = 0xaf11e000;
constexpr int FILE_VERSION = 1;

static void serialize_sequence (QDataStream &, const Sequence::ptr &);
static void deserialize_sequence (QDataStream &, const Sequence::ptr &, int);

namespace {
typedef struct
{
  char id[32];
  int version;
  int magic;
} __attribute__((packed)) FileHeader;

typedef struct
{
  int type;
  int count;
} __attribute__((packed)) NodeHeader;

typedef struct
{
  int frame_index;
} __attribute__((packed)) ItemHeader;

inline Item::ptr
deserialize_bitmap (QDataStream &stream)
{
  QImage i;

  stream >> i;
  return Bitmap::make (i);
}

inline Item::ptr
deserialize_camera (QDataStream &stream)
{
  QVector3D p, r;

  stream >> p;
  stream >> r;
  return Camera::make (p, r);
}

inline Item::ptr
deserialize_point3d (QDataStream &stream)
{
  QVector3D p;
  QColor    c;

  stream >> p;
  stream >> c;
  return Point3D::make (p, c);
}

inline Item::ptr
deserialize_cloud (QDataStream &stream)
{
  QVector<QVector3D> points;
  QVector<QColor>    colors;
  stream >> points;
  stream >> colors;
  return PointCloud::make (points, colors);
}

inline void
serialize_bitmap (QDataStream &stream, const Item::ptr & i)
{
  stream << i.dynamicCast<Bitmap> ()->get ();
}

inline void
serialize_camera (QDataStream &stream, const Item::ptr &i)
{
  Camera::ptr c = i.dynamicCast<Camera> ();

  stream << c->position ();
  stream << c->rotation ();
}

inline void
serialize_point3d (QDataStream &stream, const Item::ptr &i)
{
  Point3D::ptr p = i.dynamicCast<Point3D> ();

  stream << p->position ();
  stream << p->color ();
}

inline void
serialize_cloud (QDataStream &stream, const Item::ptr &i)
{
  PointCloud::ptr p = i.dynamicCast<PointCloud> ();

  stream << p->points ();
  stream << p->colors ();
}
}

bool
serialize_to_file (const QString & path,
                   QList<RepositoryNode *> & nodes)
{
  QFile output (path);

  if (!output.open (QFile::WriteOnly))
    return false;
  QDataStream stream (&output);
  FileHeader  fh = { "CVAR Studio repository dump", FILE_VERSION, FILE_MAGIC };
  NodeHeader  nh;

  qDebug ("Serializing a list of %d nodes", nodes.count ());

  stream.writeRawData (reinterpret_cast<const char *> (&fh),
                       sizeof(FileHeader));

  for (const RepositoryNode * n : nodes)
    {
      nh.type  = n->type ();
      nh.count = n->shared_ptr ()->items ().count ();
      stream.writeRawData (reinterpret_cast<const char *> (&nh),
                           sizeof(NodeHeader));
      stream << n->name ();

      serialize_sequence (stream, n->shared_ptr ());
    }

  qDebug ("Success. File size %lld bytes", output.size ());

  output.close ();
  return true;
}

bool
deserialize_from_file (const QString &path,
                       QList<RepositoryNode *> &nodes)
{
  QFile input (path);

  if (!input.open (QFile::ReadOnly)
      || input.size () < static_cast<qint64> (sizeof(FileHeader)))
    return false;

  QDataStream   stream (&input);
  QString       name;
  FileHeader    fh;
  NodeHeader    nh;
  Sequence::ptr sequence;

  stream.readRawData (reinterpret_cast<char *> (&fh), sizeof(FileHeader));

  qDebug ("Opening file with version=%d, magic=%x, id=%s", fh.version, fh.magic,
          fh.id);

  if (fh.version != FILE_VERSION || fh.magic != FILE_MAGIC)
    {
      qWarning ("Cannot deserialize imcompatible file!");
      return false;
    }

  while (!stream.atEnd ())
    {
      stream.readRawData (reinterpret_cast<char *> (&nh), sizeof(NodeHeader));
      stream >> name;
      sequence = Sequence::make (static_cast<Sequence::ItemType> (nh.type));
      deserialize_sequence (stream, sequence, nh.count);
      nodes.append (new RepositoryNode (name, sequence));
    }

  qDebug () << "Deserialized successfully";
  return true;
}

void
serialize_sequence (QDataStream & stream,
                    const Sequence::ptr & s)
{
  ItemHeader header;

  void (* serializer) (QDataStream &, const Item::ptr &);

  switch (s->type ())
    {
    case Sequence::Bitmap: serializer = &serialize_bitmap; break;

    case Sequence::Camera: serializer = &serialize_camera; break;

    case Sequence::Point: serializer = &serialize_point3d; break;

    case Sequence::PointCloud: serializer = &serialize_cloud; break;

    default:
      qFatal ("io: attempt to serialize an invalid sequence");
      break;
    }

  for (const Item::ptr & v : s->items ().values ())
    {
      header.frame_index = s->items ().key (v);
      stream.writeRawData (reinterpret_cast<const char *> (&header),
                           sizeof(ItemHeader));
      serializer (stream, v);
    }
}

void
deserialize_sequence (QDataStream &stream, const Sequence::ptr &s,
                      int count)
{
  ItemHeader header;
  Item::ptr  (* deserializer) (QDataStream &);

  switch (s->type ())
    {
    case Sequence::Bitmap: deserializer = &deserialize_bitmap; break;

    case Sequence::Camera: deserializer = &deserialize_camera; break;

    case Sequence::Point: deserializer = &deserialize_point3d; break;

    case Sequence::PointCloud: deserializer = &deserialize_cloud; break;

    default:
      qFatal ("io: attempt to deserialize an invalid sequence");
      break;
    }

  while (count--)
    {
      stream.readRawData (reinterpret_cast<char *> (&header),
                          sizeof(ItemHeader));
      s->add_item (header.frame_index, deserializer (stream));
    }
}
}
}
