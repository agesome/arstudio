#ifndef BITMAP_HPP
#define BITMAP_HPP

#include <opencv2/highgui/highgui.hpp>

#include <Sequence.hpp>

namespace arstudio {
class Bitmap : public Item
{
  Q_OBJECT
public:
  typedef QSharedPointer<Bitmap> ptr;
  static ptr
  make (void)
  {
    return ptr (new Bitmap);
  }

  cv::Mat bitmap;
};
}

#endif // BITMAP_HPP
