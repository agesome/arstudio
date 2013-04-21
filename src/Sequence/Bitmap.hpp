#ifndef BITMAP_HPP
#define BITMAP_HPP

#include <opencv2/highgui/highgui.hpp>

#include <Sequence.hpp>

namespace arstudio {
class Bitmap : public Item
{
public:
  typedef std::shared_ptr<Bitmap> ptr;
  static ptr
  make (void)
  {
    return std::make_shared<Bitmap> ();
  };

  cv::Mat bitmap;
};
}

#endif // BITMAP_HPP
