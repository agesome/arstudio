#include <IWManager.hpp>

namespace arstudio {
IWManager::IWManager (QObject * parent)
  : QObject (parent),
  m_viewport (nullptr),
  m_camera_mesh (nullptr),
  m_camera_effect (nullptr),
  m_path_mesh (nullptr),
  m_path_effect (nullptr),
  m_line_effect (nullptr),
  m_scenegraph (Scenegraph::make ()),
  m_current_frame (1)
{
  ScenegraphAggregator * sa = ScenegraphAggregator::instance ();

  connect (m_scenegraph.data (), &Scenegraph::sequences_changed,
           [this] () { paint_frame (m_current_frame); });
  sa->add_scenegraph (m_scenegraph.data ());
  connect (sa, &ScenegraphAggregator::change_frame,
           this, &IWManager::paint_frame);
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
  // always created here, can check just one
  if (m_camera_effect)
    {
      delete m_camera_effect;
      delete m_camera_mesh;
      delete m_path_effect;
      delete m_path_mesh;
      delete m_line_effect;
    }

  m_camera_effect = new QQuickEffect (viewport);
  m_camera_mesh   = new CylinderMesh (viewport);
  m_path_effect   = new QQuickEffect (viewport);
  m_path_mesh     = new SphereMesh (viewport);
  m_line_effect   = new QQuickEffect (viewport);

  m_camera_effect->setColor (QColor::fromRgb (250, 0, 0));
  m_camera_mesh->setRadius (0.008);
  m_camera_mesh->setLength (0.12);

  m_path_effect->setColor (QColor::fromRgb (0, 0, 250));
  m_path_mesh->setRadius (0.008);

  m_line_effect->setColor (QColor::fromRgb (0, 250, 0));
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
  QGraphicsRotation3D * rx = new QGraphicsRotation3D (i);
  QGraphicsRotation3D * ry = new QGraphicsRotation3D (i);
  QGraphicsRotation3D * rz = new QGraphicsRotation3D (i);

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

  i->setEffect (m_camera_effect);
  i->setMesh (m_camera_mesh);
  i->setPosition (c->position ());
  i->setParentItem (m_viewport);
}

void
IWManager::add_camera_path (Sequence * s)
{
  QList<QVariant> points;
  Line          * l = new Line (m_viewport);

  for (auto ip : s->items ())
    {
      Item::ptr   i = ip.second;
      Camera::ptr c = i.dynamicCast<Camera> ();
      points << c->position ().x ();
      points << c->position ().y ();
      points << c->position ().z ();

      QQuickItem3D * p = new QQuickItem3D (m_viewport);
      p->setMesh (m_path_mesh);
      p->setEffect (m_path_effect);
      p->setPosition (c->position ());
      p->setParentItem (m_viewport);
    }

  l->setWidth (2.0);
  l->setEffect (m_line_effect);
  l->setVertices (points);
  l->setParentItem (m_viewport);
}

void
IWManager::paint_frame (int frame)
{
  Q_ASSERT (m_viewport);
#ifndef QT_NO_DEBUG
  qint64 time = QDateTime::currentMSecsSinceEpoch ();
#endif
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
#ifndef QT_NO_DEBUG
  time = QDateTime::currentMSecsSinceEpoch () - time;
  qDebug () << QString ("paint_frame() done in %0 msec").arg (time);
#endif
}
}
