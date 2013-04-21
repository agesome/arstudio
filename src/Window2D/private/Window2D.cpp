#include <Window2D.hpp>

namespace arstudio {
Window2D::Window2D (Scenegraph::ptr sg, QWidget * parent) : QLabel (parent)
{
  this->scenegraph_ptr = sg;
  this->current_frame  = 1;
}

void
Window2D::update (void)
{
  this->update (current_frame);
}

/**
 *      This method displays a Bitmap corresponding to a certain frame, if
 * there is one.
 *
 *      \param frame the frame for wich to seek a Bitmap
 */

void
Window2D::update (int frame)
{
  current_frame = frame;
  for (auto seq : scenegraph_ptr->sequences ())
    {
      if (seq->type () != Item::BITMAP)
        continue;
      Sequence::frame_map items = seq->items ();
      Item::ptr           image;

      auto it = items.find (frame);
      if (it == items.end ())
        continue;
      image = it->second;

      Bitmap::ptr bm;
      QImage      img;
      cv::Mat     rgb;

      bm = Item::ptr_cast_to<Bitmap> (image);
      cvtColor (bm->bitmap, rgb, CV_BGR2RGB);
      img = QImage (rgb.data, rgb.cols, rgb.rows, rgb.step,
                    QImage::Format_RGB888);
      curernt_pixmap = QPixmap::fromImage (img);
      setPixmap (curernt_pixmap.scaled (width (), height (),
                                        Qt::KeepAspectRatio));
      // once we found an image to display, don't look for more images
      break;
    }
}

/**
 *      This method scales currently displayed bitmap to new widget size.
 */

void
Window2D::resizeEvent (QResizeEvent *)
{
  if (curernt_pixmap.isNull ())
    return;
  setPixmap (curernt_pixmap.scaled (width (), height (), Qt::KeepAspectRatio));
}
}
