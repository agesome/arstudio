#ifndef REPOSITORY_SERIALIZER_HPP
#define REPOSITORY_SERIALIZER_HPP

#include <QFile>
#include <QString>
#include <QList>
#ifndef QT_NO_DEBUG
#include <QtDebug>
#endif

#include <RepositoryNode.hpp>
#include <Bitmap.hpp>
#include <Camera.hpp>
#include <Point3D.hpp>
#include <PointCloud.hpp>
#include <repository.pb.h>

namespace ap = arstudio_protobuf;

namespace arstudio {
class RepositoryIO
{
public:
  static bool serialize_to_file (const QString & path,
                                 QList<RepositoryNode *> & nodes);
  static bool deserialize_from_file (const QString & path,
                                     QList<RepositoryNode *> & nodes);
private:
  template <typename SEQUENCE_TYPE, typename EXT_ID_TYPE>
  static inline void
  append_items (Sequence * sequence, ap::Node * node,
                EXT_ID_TYPE & id)
  {
    ap::Item * i;

    for (const Item::ptr & item : sequence->items ())
      {
        i = node->add_items ();
        auto e = i->MutableExtension (id);
        item.dynamicCast<SEQUENCE_TYPE> ()->serialize (e);
      }
  }

  template <typename T>
  static inline Sequence::ptr
  node_to_sequence (const ap::Node & node)
  {
    Sequence::ptr s = Sequence::make (node.type ());

    for (int i = 0; i < node.items_size (); i++)
      {
        const ap::Item & it = node.items (i);
        s->add_item (i + 1, T::deserialize (it));
      }
    return s;
  }
};
}
#endif
