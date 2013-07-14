#ifndef BITMAP_HPP
#define BITMAP_HPP

#include <QImage>
#include <QBuffer>
#include <opencv2/highgui/highgui.hpp>
#include <Sequence.hpp>
#include <repository.pb.h>
#include <QtDebug>
#include <QFile>
#include <string>

namespace ap = arstudio_protobuf;

namespace arstudio {
class Bitmap : public Item
{
public:
  typedef QSharedPointer<Bitmap> ptr;

  Bitmap (QImage p)
  {
    m_image = p.copy ();
  }

  static ptr
  make (QImage & p)
  {
    return ptr (new Bitmap (p));
  }

  QImage
  get ()
  {
    return m_image;
  }

  inline void
  serialize (ap::Bitmap * bm)
  {
    QBuffer buf;

    buf.open (QBuffer::WriteOnly);
    m_image.save (&buf, "PNG");
    buf.close ();
    bm->set_allocated_data (new std::string (buf.data ().data (),
                                             buf.data ().size ()));
  }

  static inline Item::ptr
  deserialize (const ap::Item & i)
  {
    ap::Bitmap bm = i.GetExtension (ap::Bitmap::bitmap);
    QImage     img;

    img.loadFromData (reinterpret_cast<const uchar *> (bm.data ().c_str ()),
                      bm.data ().size (), "PNG");
    return Bitmap::make (img);
  }

private:
  QImage m_image;
};
}

#endif // BITMAP_HPP
