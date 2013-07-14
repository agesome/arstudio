#ifndef BITMAP_HPP
#define BITMAP_HPP

#include <string>

#include <QImage>
#include <QBuffer>

#include <opencv2/highgui/highgui.hpp>

#include <Sequence.hpp>

namespace arstudio {
class Bitmap : public Item
{
public:
  typedef QSharedPointer<Bitmap> ptr;

  Bitmap (QImage p);

  static ptr
  make (QImage & p);

  QImage
  get ();

  void
  serialize (ap::Bitmap * bm);

  static Item::ptr
  deserialize (const ap::Item & i);

private:
  QImage m_image;
};
}

#endif // BITMAP_HPP
