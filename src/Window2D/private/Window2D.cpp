#include <Window2D.hpp>

Window2D::Window2D (Scenegraph::ptr sg, QWidget * parent) : QLabel (parent)
{
  this->scenegraph = sg;
}

void
Window2D::update (void)
{
  this->update (currentFrame);
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
  cv::Mat rgb;

  currentFrame = frame;
  for (auto seq : scenegraph->sequences ())
    {
      if (seq->type () != Item::BITMAP)
        continue;
      Sequence::frame_map items = seq->items ();
      Item::ptr           image;
      // seems cleaner than using find()
      try
        {
          image = items.at (frame);
        }
      catch (std::out_of_range ex)
        {
          continue;
        }

      Bitmap::ptr bm = Item::ptr_cast_to<Bitmap> (image);
      cvtColor (bm->bitmap, rgb, CV_BGR2RGB);
      currentPixmap = QPixmap::fromImage (QImage (rgb.data, rgb.cols, rgb.rows,
                                                  rgb.step,
                                                  QImage::Format_RGB888));
      this->setPixmap (currentPixmap.scaled (this->width (), this->height (),
                                             Qt::KeepAspectRatio));
    }
}

/**
 *      This method scales currently displayed bitmap to new widget size.
 */

void
Window2D::resizeEvent (QResizeEvent *)
{
  if (currentPixmap.isNull ())
    return;
  this->setPixmap (currentPixmap.scaled (this->width (), this->height (),
                                         Qt::KeepAspectRatio));
}
