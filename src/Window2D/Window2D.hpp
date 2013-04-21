#ifndef WINDOW2D_H
#define WINDOW2D_H

#include <QImage>
#include <QLabel>

#include <opencv2/imgproc/imgproc.hpp>

#include <Scenegraph.hpp>
#include <Bitmap.hpp>

namespace arstudio {
/**
 *      This widget displays items of type Bitmap.
 */

class Window2D : public QLabel
{
  Q_OBJECT
public:
  Window2D (Scenegraph::ptr, QWidget * parent = nullptr);
private:
  void resizeEvent (QResizeEvent *);

  QPixmap         curernt_pixmap; //< currently displayed QPixmap
  Scenegraph::ptr scenegraph_ptr; //< an instance of Scenegraph
  int             current_frame; //< currently used frame
public slots:
  void update (int);
  void update (void);
};
}

#endif // WINDOW2D_H
