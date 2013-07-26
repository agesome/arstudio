#include <BitmapView.hpp>

namespace arstudio {
BitmapView::BitmapView (QQuickItem * parent)
  : QQuickItem (parent)
{
  connect (this, &BitmapView::windowChanged, this, &BitmapView::window_set);
}

void
BitmapView::set_image (const QImage & i)
{
  if (i.isNull ())
    return;
  m_image = i;
  setFlag (ItemHasContents, true);
  update ();
}

QSGNode *
BitmapView::updatePaintNode (QSGNode * node, UpdatePaintNodeData *)
{
  QImage                 scaled;
  QSGSimpleTextureNode * n = static_cast<QSGSimpleTextureNode *> (node);
  QSGTexture           * old_texture;
  int                    w = width ();
  int                    h = height ();

  if (!n)
    n = new QSGSimpleTextureNode;

  old_texture = n->texture ();
  if (old_texture)
    {
      n->setTexture (nullptr);
      delete old_texture;
    }

  scaled = m_image.scaled (w, h, Qt::KeepAspectRatio,
                           Qt::SmoothTransformation);
  n->setTexture (window ()->createTextureFromImage (scaled));
  n->setRect (QRect (0, 0, scaled.width (), scaled.height ()));

  return n;
}

void
BitmapView::window_set (QQuickWindow * w)
{
  // at exit, something calls this with w = null; why? is that normal?
  if (!w)
    return;
  connect (w, &QQuickWindow::widthChanged, this, &BitmapView::update);
  connect (w, &QQuickWindow::heightChanged, this, &BitmapView::update);
}
}
