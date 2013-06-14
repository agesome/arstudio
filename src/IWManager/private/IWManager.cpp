#include <IWManager.hpp>

namespace arstudio {
IWManager::IWManager (QObject * parent)
  : QObject (parent),
  m_viewport (nullptr),
  m_camera (nullptr),
  m_cylinder_mesh (nullptr),
  m_camera_effect (nullptr),
  m_sphere_mesh (nullptr),
  m_path_effect (nullptr),
  m_line_effect (nullptr),
  m_scenegraph (Scenegraph::make ()),
  m_current_frame (1),
  m_camera_view (false),
  m_camera_view_distance (1.5)
{
  ScenegraphAggregator * sa = ScenegraphAggregator::instance ();

  connect (m_scenegraph.data (), &Scenegraph::sequences_changed,
           this, &IWManager::repaint_frame);
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
      delete m_cylinder_mesh;
      delete m_path_effect;
      delete m_sphere_mesh;
      delete m_line_effect;
    }

  m_camera_effect = new QQuickEffect (viewport);
  m_cylinder_mesh = new CylinderMesh (viewport);
  m_path_effect   = new QQuickEffect (viewport);
  m_sphere_mesh   = new SphereMesh (viewport);
  m_line_effect   = new QQuickEffect (viewport);

  m_camera_effect->setColor (QColor::fromRgb (250, 0, 0));
  m_cylinder_mesh->setRadius (0.008);
  m_cylinder_mesh->setLength (0.12);

  m_path_effect->setColor (QColor::fromRgb (0, 0, 250));
  m_sphere_mesh->setRadius (0.008);

  m_line_effect->setColor (QColor::fromRgb (0, 250, 0));
}

QGLCamera *
IWManager::camera ()
{
  return m_camera;
}

void
IWManager::set_camera (QGLCamera * camera)
{
  m_camera = camera;
}

bool
IWManager::camera_view ()
{
  return m_camera_view;
}

void
IWManager::set_camera_view (bool v)
{
  m_camera_view = v;
  if (!v)
    reset_camera ();
  else
    // set near plane for close object viewing
    m_camera->setNearPlane (1.0);
  repaint_frame ();
}

QVector3D
IWManager::camera_view_position ()
{
  return m_camera_view_position;
}

qreal
IWManager::camera_view_distance ()
{
  return m_camera_view_distance;
}

void
IWManager::set_camera_view_distance (qreal d)
{
  m_camera_view_distance = d;
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
  i->setMesh (m_cylinder_mesh);
  i->setPosition (c->position ());
  i->setParentItem (m_viewport);
}

void
IWManager::add_camera_path (Sequence * s)
{
  QList<QVector3D> points;
  QVector3D        p;

  for (auto ip : s->items ())
    {
      Item::ptr   i = ip.second;
      Camera::ptr c = i.dynamicCast<Camera> ();

      p = c->position ();
      points << p;
      add_sphere (p);
    }

  add_line (points);
}

void
IWManager::add_line (const QList<QVariant> & vertices)
{
  Line * l = new Line (m_viewport);

  l->setEffect (m_line_effect);
  l->setWidth (2.0);
  l->setVertices (vertices);
  l->setParentItem (m_viewport);
}

void
IWManager::add_line (const QList<QVector3D> & points)
{
  QList<QVariant> vertices;
  for (auto v : points)
    {
      vertices << v.x ();
      vertices << v.y ();
      vertices << v.z ();
    }
  add_line (vertices);
}

void
IWManager::add_sphere (const QVector3D & location)
{
  QQuickItem3D * s = new QQuickItem3D (m_viewport);

  s->setMesh (m_sphere_mesh);
  s->setEffect (m_path_effect);
  s->setPosition (location);
  s->setParentItem (m_viewport);
}

void
IWManager::look_from_camera (Camera::ptr c)
{
  const float x = (c->rotation ().x () / 180) * M_PI;
  const float y = (c->rotation ().y () / 180) * M_PI;

  const float c1 = std::cos (x);
  const float s1 = std::sin (x);
  const float c2 = std::cos (y);
  const float s2 = std::sin (y);

  // direction vector, aligned with camera
  QVector3D camera_direction (s2, -c2 * s1, c2 * c1);
  QRay3D    camera_direction_ray (c->position (), camera_direction);

  m_camera_view_position = camera_direction_ray.point (m_camera_view_distance);
  camera_view_position_changed ();

  QPlane3D  ground (QVector3D (0, 0, 0), QVector3D (0, 1, 0));
  qreal     it = ground.intersection (camera_direction_ray);
  QVector3D ip = camera_direction_ray.point (it);
  ip.setY (0);
  ip -= camera_direction_ray.point (it + 1);
  QVector3D ipp (-ip.z (), 0, ip.x ());
  QRay3D    ipp_ray (camera_direction_ray.point (it), ipp);

  QPlane3D view_plane (c->position (),
                       camera_view_position (),
                       ipp_ray.point (1.0));

  m_camera->setCenter (m_camera_view_position);
  m_camera->setEye (c->position ());
  m_camera->setUpVector (view_plane.normal ());
}

void
IWManager::reset_camera (const IWManager::Axis axis)
{
  switch (axis)
    {
    case X:
      m_camera->setEye (QVector3D (10, 0, 0));
      m_camera->setCenter (QVector3D (0, 0, 0));
      m_camera->setUpVector (QVector3D (0, 1, 0));
      break;

    case Y:
      m_camera->setEye (QVector3D (0, 10, 0));
      m_camera->setCenter (QVector3D (0, 0, 0));
      m_camera->setUpVector (QVector3D (1, 0, 0));

    case Z:
      m_camera->setEye (QVector3D (0, 0, 10));
      m_camera->setCenter (QVector3D (0, 0, 0));
      m_camera->setUpVector (QVector3D (0, 1, 0));
    }
  m_camera->setNearPlane (5.0);
}

void
IWManager::paint_frame (int frame)
{
  Q_ASSERT (m_viewport);
  Camera::ptr eye_camera;

#ifndef QT_NO_DEBUG
  qint64 time = QDateTime::currentMSecsSinceEpoch ();
#endif
  QList<QQuickItem *> ci = m_viewport->childItems ();
  for (QQuickItem * i : ci)
    {
      if (i->objectName ().contains ("vp_ignore"))
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
          eye_camera = p.dynamicCast<Camera> ();
          add_camera (eye_camera);
          add_camera_path (s);
          break;

        default:
          return;
        }
    }

  if (m_camera_view && m_camera && eye_camera)
    look_from_camera (eye_camera);

  m_viewport->update ();
  m_current_frame = frame;
#ifndef QT_NO_DEBUG
  time = QDateTime::currentMSecsSinceEpoch () - time;
  qDebug () << QString ("paint_frame() done in %0 msec").arg (time);
#endif
}

void
IWManager::repaint_frame (void)
{
  paint_frame (m_current_frame);
}

void
IWManager::viewport_keypress (const QKeyEvent e)
{
  switch (e.key ())
    {
    case Qt::Key_X:
      reset_camera (X);
      break;

    case Qt::Key_Y:
      reset_camera (Y);
      break;

    case Qt::Key_Z:
    case Qt::Key_Space:
      reset_camera (Z);
      break;
    }
}
}
