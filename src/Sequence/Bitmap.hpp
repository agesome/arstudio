#ifndef BITMAP_HPP
#define BITMAP_HPP

#include <QImage>
#include <opencv2/highgui/highgui.hpp>
#include <Sequence.hpp>

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

private:
  QImage m_image;
};
}

#endif // BITMAP_HPP
