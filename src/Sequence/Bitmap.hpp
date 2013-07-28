#ifndef BITMAP_HPP
#define BITMAP_HPP

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
private:
  QImage m_image;
};
}

#endif // BITMAP_HPP
