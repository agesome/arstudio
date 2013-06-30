#ifndef BITMAPVIEW_HPP
#define BITMAPVIEW_HPP

#include <QQuickItem>
#include <QSGSimpleTextureNode>
#include <QQuickWindow>

namespace arstudio {
/**
 * @brief Convenience class for displaying a QImage in QtQuick
 */
class BitmapView : public QQuickItem
{
  Q_OBJECT
  Q_PROPERTY (QImage image READ image WRITE set_image)
public:
  BitmapView (QQuickItem * parent = nullptr);

  void set_image (const QImage &);
  QImage
  image ()
  {
    return m_image;
  }

protected:
  QSGNode * updatePaintNode (QSGNode *, UpdatePaintNodeData *);
private:
  QImage m_image;
private slots:
  void window_set (QQuickWindow *);
};
}

#endif
