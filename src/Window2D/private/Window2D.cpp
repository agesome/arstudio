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

void
Window2D::update (int frame)
{
	cv::Mat rgb;

	currentFrame = frame;
	for (auto seq : scenegraph->getSequences ())
		{
			if (seq->getType () != Item::BITMAP)
				continue;
			Sequence::map items = seq->getItems ();
			Item::ptr image;
			// seems cleaner than using find()
			try
				{
					image = items.at (frame);
				}
			catch (out_of_range ex)
				{
					this->hide ();
					return;
				}

			Bitmap::ptr bm = Item::ptr_cast_to<Bitmap> (image);
			cvtColor (bm->bitmap, rgb, CV_BGR2RGB);
			currentPixmap = QPixmap::fromImage (QImage (rgb.data, rgb.cols, rgb.rows,
			                                            rgb.step, QImage::Format_RGB888));
			this->setPixmap (currentPixmap.scaled (this->width (), this->height (),
			                                       Qt::KeepAspectRatio));
			this->show ();
			return;
		}
	this->hide ();
}

void
Window2D::resizeEvent (QResizeEvent *)
{
	if (currentPixmap.isNull ())
		return;
	this->setPixmap (currentPixmap.scaled (this->width (), this->height (),
	                                       Qt::KeepAspectRatio));
}
