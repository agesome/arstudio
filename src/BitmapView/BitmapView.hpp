#ifndef BITMAPVIEW_HPP
#define BITMAPVIEW_HPP

#include <QQuickItem>
#include <QSGSimpleTextureNode>
#include <QQuickWindow>

namespace arstudio {
class BitmapView : public QQuickItem
{
  Q_OBJECT
public:
  BitmapView (QQuickItem * parent = nullptr);

  void update_image (const QImage &);

protected:
  QSGNode * updatePaintNode (QSGNode *, UpdatePaintNodeData *);
private:
  QImage m_image;
private slots:
  void window_set (QQuickWindow *);
};
}

#endif
