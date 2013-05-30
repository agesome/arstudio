#include <IWManager.hpp>

namespace arstudio {
IWManager::IWManager (QObject * parent)
  : QObject (parent),
  m_viewport (nullptr),
  m_scenegraph (Scenegraph::make ()),
  m_current_frame (10)
{
  connect (m_scenegraph.data (), &Scenegraph::sequences_changed,
           [this] () { paint_frame (m_current_frame); });
}

QQuickPaintedItem *
IWManager::viewport (void)
{
  return m_viewport;
}

void
IWManager::set_viewport (QQuickPaintedItem * viewport)
{
  m_viewport = viewport;
}

Scenegraph *
IWManager::scenegraph (void)
{
  Scenegraph * p = m_scenegraph.data ();
  QQmlEngine::setObjectOwnership (p, QQmlEngine::CppOwnership);

  return p;
}

void
IWManager::add_camera (Camera::ptr c)
{
  QQuickItem3D        * i  = new QQuickItem3D (m_viewport);
  CylinderMesh        * m  = new CylinderMesh (i);
  QQuickEffect        * e  = new QQuickEffect (i);
  QGraphicsRotation3D * rx = new QGraphicsRotation3D (i);
  QGraphicsRotation3D * ry = new QGraphicsRotation3D (i);
  QGraphicsRotation3D * rz = new QGraphicsRotation3D (i);

  e->setColor (QColor::fromRgb (255, 0, 0));

  rx->setAxis (QVector3D (1, 0, 0));
  rx->setAngle (c->rotation ().x ());
  ry->setAxis (QVector3D (0, 1, 0));
  ry->setAngle (c->rotation ().y ());
  rz->setAxis (QVector3D (0, 0, 1));
  rz->setAngle (c->rotation ().z ());

  QQmlListProperty<QQuickQGraphicsTransform3D> p = i->transform ();
  p.append (&p, rx);
  p.append (&p, ry);
  p.append (&p, rz);

  m->setRadius (0.008);
  m->setLength (0.12);

  i->setEffect (e);
  i->setMesh (m);
  i->setPosition (c->position ());

  i->setParentItem (m_viewport);
}

void
IWManager::add_camera_path (Sequence * s)
{
  Q_ASSERT (s->type () == Sequence::CAMERA);

  QList<QVariant> points;
  Line          * l = new Line (m_viewport);
  QQuickEffect  * e = new QQuickEffect (l);

  for (auto ip : s->items ())
    {
      Item::ptr   i = ip.second;
      Camera::ptr c = i.dynamicCast<Camera> ();
      points << c->position ().x ();
      points << c->position ().y ();
      points << c->position ().z ();

      QQuickItem3D * p  = new QQuickItem3D (m_viewport);
      QQuickEffect * pe = new QQuickEffect (p);
      SphereMesh   * pm = new SphereMesh (p);
      pe->setColor (QColor::fromRgb (0, 0, 250));
      pm->setRadius (0.008);
      p->setMesh (pm);
      p->setEffect (pe);
      p->setPosition (c->position ());
      p->setParentItem (m_viewport);
    }

  e->setColor (QColor::fromRgb (0, 250, 0));
  l->setWidth (2.0);
  l->setEffect (e);
  l->setVertices (points);
  l->setParentItem (m_viewport);
}

void
IWManager::paint_frame (int frame)
{
  Q_ASSERT (m_viewport);
  // TODO: optimize to add/remove only changed sequences
  // if painting same frame
  QList<QQuickItem *> ci = m_viewport->childItems ();
  for (QQuickItem * i : ci)
    {
      if (i->objectName () == "vp_axis")
        continue;
      delete i;
    }

  for (Sequence * s : m_scenegraph->sequences ())
    {
      Item::ptr p = s->item_for_frame (frame);
      if (!p)
        continue;
      switch (s->type ())
        {
        case Sequence::CAMERA:
          add_camera (p.dynamicCast<Camera> ());
          add_camera_path (s);
          break;

        default:
          return;
        }
    }
  m_viewport->update ();
  m_current_frame = frame;
}
}
