#include <ItemView.hpp>

#define DEBUG_RENDERING 0

namespace arstudio {
// anonymous namespace with utility functions
namespace {
inline osg::Vec3
qvec2osg (const QVector3D & v)
{
  return osg::Vec3 (v.x (), v.y (), v.z ());
}
}

ItemView::ItemView (QQuickItem * parent)
  : QQuickItem (parent)
  , m_scenegraph (Scenegraph::make ())
  , m_sequence_node (nullptr)
  , m_current_frame (1)
  , m_show_camera_path (true)
  , m_qt_opengl_ctx (nullptr)
  , m_osg_opengl_ctx (nullptr)
  , m_qt_texture (nullptr)
  , m_qt_geometry (QSGGeometry::defaultAttributes_TexturedPoint2D (), 4)
  , m_osg_texture_object (nullptr)
  , m_size_valid (false)
  , m_osg_initialized (false)
{
  m_geometry_node.setGeometry (&m_qt_geometry);
  m_geometry_node.setMaterial (&m_qt_tex_material);
  m_geometry_node.setOpaqueMaterial (&m_qt_opaque_material);

  setFlag (ItemHasContents);
  setAntialiasing (true);
  setAcceptedMouseButtons (Qt::MouseButton::LeftButton);

  // we want to redraw when: a) there may be new sequences for current frame
  // and b) when the frame index changes
  ScenegraphAggregator::instance ()->add_scenegraph (m_scenegraph.data ());
  connect (m_scenegraph.data (), &Scenegraph::sequences_changed,
           this, &ItemView::update_scene);
  connect (ScenegraphAggregator::instance (),
           &ScenegraphAggregator::change_frame,
           this, &ItemView::change_frame);

  // prevent the Qt Scenegraph from trying to delete our node
  m_geometry_node.setFlag (QSGGeometryNode::OwnedByParent, false);
}

ItemView::~ItemView ()
{
  if (ScenegraphAggregator::instance ())
    ScenegraphAggregator::instance ()->remove_scenegraph (m_scenegraph.data ());

}

Scenegraph *
ItemView::scenegraph ()
{
  QQmlEngine::setObjectOwnership (m_scenegraph.data (),
                                  QQmlEngine::CppOwnership);

  return m_scenegraph.data ();
}

bool
ItemView::show_camera_path ()
{
  return m_show_camera_path;
}

void
ItemView::set_show_camera_path (bool v)
{
  m_show_camera_path = v;
  show_camera_path_changed ();
  update ();
}

void
ItemView::paint ()
{
#if DEBUG_RENDERING
  static QElapsedTimer call_timer;
  call_timer.restart ();
#endif

  // create the initial OSG objects and the axis arrows
  if (!m_osg_initialized)
    {
      init ();
      m_osg_initialized = true;
    }

  // viewer/camera setup on item resize
  if (!m_size_valid)
    {
      m_osg_viewer->setUpViewerAsEmbeddedInWindow (0, 0, width (), height ());
      m_osg_texture->setTextureSize (width (), height ());
      m_osg_texture->dirtyTextureObject ();
      m_osg_viewer->getCamera ()->setRenderingCache (nullptr);
      m_osg_viewer->getCamera ()->setViewport (0, 0, width (), height ());

      osgViewer::Renderer * renderer
        = (osgViewer::Renderer *) m_osg_viewer->getCamera ()->getRenderer ();
      renderer->getSceneView (0)->getRenderStage ()->setFrameBufferObject (NULL);
      renderer->getSceneView (0)->getRenderStage ()->setCameraRequiresSetUp (
        true);
    }

  // save Qt context, make OSG context current
  m_qt_opengl_ctx = QOpenGLContext::currentContext ();
  m_qt_opengl_ctx->doneCurrent ();
  m_osg_opengl_ctx->makeCurrent (window ());

  if (!m_osg_viewer->isRealized ())
    m_osg_viewer->realize ();

  // actual OSG rendering happens here
  m_osg_viewer->frame ();

  // restore Qt context
  m_osg_opengl_ctx->doneCurrent ();
  m_qt_opengl_ctx->makeCurrent (window ());

  // this is done on first run to create a texture of correct size and also
  // on eash resize of this item to re-create the texture
  if (!m_size_valid)
    {
      QSGGeometry::updateTexturedRectGeometry (&m_qt_geometry,
                                               QRectF (0, 0, width (),
                                                       height ()),
                                               QRectF (0, 1, 1, -1));

      unsigned int ctx_id = m_osg_viewer->getCamera ()->getGraphicsContext ()
                            ->getState ()->getContextID ();
      m_osg_texture_object = m_osg_texture->getTextureObject (ctx_id);
      Q_ASSERT (m_osg_texture_object);

      if (m_qt_texture)
        delete m_qt_texture;
      m_qt_texture = window ()->createTextureFromId (
        m_osg_texture_object->id (),
        QSize (width (),
               height ()));
      m_qt_tex_material.setTexture (m_qt_texture);
      m_qt_opaque_material.setTexture (m_qt_texture);

      m_size_valid = true;
    }
#if DEBUG_RENDERING
  qDebug ("paint() done in %lld us", call_timer.nsecsElapsed () / 1000);
#endif
}

void
ItemView::update_scene ()
{
#if DEBUG_RENDERING
  static QElapsedTimer call_timer;
  call_timer.restart ();
#endif

  if (m_sequence_node)
    m_osg_scene->removeChild (m_sequence_node);
  m_sequence_node = new osg::Geode;
  m_osg_scene->addChild (m_sequence_node);

  for (const Sequence * seq : m_scenegraph->sequences ())
    {
      Item::ptr item = seq->item_for_frame (m_current_frame);
      switch (seq->type ())
        {
        case Sequence::Camera:
          add_camera (item.dynamicCast<Camera> ());
          if (m_show_camera_path)
            add_camera_path (seq);
          break;
        }
    }

#if DEBUG_RENDERING
  qDebug ("update_scene() done in %lld us", call_timer.nsecsElapsed () / 1000);
#endif

  // initiate redraw
  update ();
}

void
ItemView::add_camera (const Camera::ptr camera)
{
  osg::ShapeDrawable * s;
  osg::Cone          * c;
  osg::Quat            r;

  c = new osg::Cone (qvec2osg (camera->position ()), 0.03, 0.06);

  //  FIXME: rotation conversion is probably wrong
  r = osg::Quat (camera->rotation ().x (), osg::Vec3f (1, 0, 0)) +
      osg::Quat (camera->rotation ().y (), osg::Vec3f (0, 1, 0)) +
      osg::Quat (camera->rotation ().z (), osg::Vec3f (0, 0, 1));

  c->setRotation (r);

  s = new osg::ShapeDrawable (c);
  s->setColor (osg::Vec4 (1, 0, 0, 1));
  m_sequence_node->addDrawable (s);
}

void
ItemView::add_camera_path (const Sequence * sequence)
{
  Q_ASSERT (sequence->type () == Sequence::Camera);

  Camera::ptr          c, prev;
  osg::Geometry      * geometry = new osg::Geometry;
  osg::Vec3Array     * points   = new osg::Vec3Array;
  osg::Vec4Array     * color    = new osg::Vec4Array;
  osg::LineWidth     * lw       = new osg::LineWidth (2);
  osg::Sphere        * sphere;
  osg::ShapeDrawable * sphere_drawable;

  color->push_back (osg::Vec4 (0, 1, 0, 1));

  for (auto p : sequence->items ())
    {
      c = p.dynamicCast<Camera> ();

      sphere          = new osg::Sphere (qvec2osg (c->position ()), .01);
      sphere_drawable = new osg::ShapeDrawable (sphere);
      m_sequence_node->addDrawable (sphere_drawable);
      if (prev)
        {
          points->push_back (qvec2osg (prev->position ()));
          points->push_back (qvec2osg (c->position ()));
        }
      prev = c;
    }

  geometry->setVertexArray (points);
  geometry->setColorArray (color);
  geometry->setColorBinding (osg::Geometry::BIND_PER_PRIMITIVE_SET);
  geometry->addPrimitiveSet (new osg::DrawArrays (GL_LINES, 0,
                                                  points->size ()));
  geometry->getOrCreateStateSet ()->setAttributeAndModes (lw);
  m_sequence_node->addDrawable (geometry);
}

void
ItemView::change_frame (int frame)
{
  m_current_frame = frame;
  update_scene ();
}

void
ItemView::init ()
{
  // create a separate OpenGL context for OSG
  m_osg_opengl_ctx = new QOpenGLContext;
  m_osg_opengl_ctx->setShareContext (QOpenGLContext::currentContext ());
  Q_ASSERT (m_osg_opengl_ctx->create ());

  m_osg_viewer = new osgViewer::Viewer;

  osg::Light * light = m_osg_viewer->getLight ();
  light->setAmbient (osg::Vec4 (.2, .2, .2, .2));

  osg::Camera * camera = m_osg_viewer->getCamera ();
  camera->setRenderTargetImplementation (osg::Camera::FRAME_BUFFER_OBJECT);
  // background color for whatever is rendered with OSG
  camera->setClearColor (osg::Vec4 (.05, .05, .05, 1.0));

  m_osg_texture = new osg::Texture2D;
  m_osg_texture->setInternalFormat (GL_RGBA);
  m_osg_texture->setFilter (osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
  m_osg_texture->setFilter (osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);

  camera->attach (osg::Camera::COLOR_BUFFER, m_osg_texture.get ());

  m_osg_orbit = new osgGA::OrbitManipulator;
  m_osg_viewer->setCameraManipulator (m_osg_orbit.get ());
  m_osg_orbit->setHomePosition (osg::Vec3 (3, 0, 3), osg::Vec3 (0, 0, 0),
                                osg::Vec3 (0, 0, 1));
  m_osg_viewer->home ();

  m_osg_scene = new osg::Group;
  m_osg_viewer->setSceneData (m_osg_scene.get ());

  osg::Geode         * axis = new osg::Geode;
  osg::ShapeDrawable * s;
  osg::Cylinder      * cyl;
  osg::Cone          * cone;
  // cylinder radius
  constexpr float cr = 0.01;
  // cone height
  constexpr float ch = 8 * cr;

  // Z arrow
  cyl = new osg::Cylinder (osg::Vec3 (0, 0, 0.5), cr, 1);
  s   = new osg::ShapeDrawable (cyl);
  axis->addDrawable (s);
  cone = new osg::Cone (osg::Vec3 (0, 0, 1), 2 * cr, ch);
  s    = new osg::ShapeDrawable (cone);
  s->setColor (osg::Vec4 (0, 0, 1, 1.0));
  axis->addDrawable (s);

  // X arrow
  cyl = new osg::Cylinder (osg::Vec3 (0.5, 0, 0), cr, 1);
  s   = new osg::ShapeDrawable (cyl);
  cyl->setRotation (osg::Quat (M_PI_2, osg::Vec3 (0, 1, 0)));
  axis->addDrawable (s);
  cone = new osg::Cone (osg::Vec3 (1, 0, 0), 2 * cr, ch);
  s    = new osg::ShapeDrawable (cone);
  cone->setRotation (osg::Quat (M_PI_2, osg::Vec3 (0, 1, 0)));
  s->setColor (osg::Vec4 (0, 1, 0, 1.0));
  axis->addDrawable (s);

  // Y arrow
  cyl = new osg::Cylinder (osg::Vec3 (0, 0.5, 0), cr, 1);
  s   = new osg::ShapeDrawable (cyl);
  cyl->setRotation (osg::Quat (M_PI_2, osg::Vec3 (1, 0, 0)));
  axis->addDrawable (s);
  cone = new osg::Cone (osg::Vec3 (0, 1, 0), 2 * cr, ch);
  s    = new osg::ShapeDrawable (cone);
  cone->setRotation (osg::Quat (-M_PI_2, osg::Vec3 (1, 0, 0)));
  s->setColor (osg::Vec4 (1, 0, 0, 1.0));
  axis->addDrawable (s);

  m_osg_scene->addChild (axis);
}

QSGNode *
ItemView::updatePaintNode (QSGNode *, QQuickItem::UpdatePaintNodeData *)
{
  paint ();
  m_geometry_node.markDirty (QSGGeometryNode::DirtyMaterial);
  return &m_geometry_node;
}

void
ItemView::wheelEvent (QWheelEvent * event)
{
  const double d  = m_osg_orbit->getDistance ();
  const double dx = event->angleDelta ().y () / 240.0;

  m_osg_orbit->setDistance (d - dx);
  update ();
  event->accept ();
}

void
ItemView::mouseMoveEvent (QMouseEvent * event)
{
  if (m_mouse_buttons & Qt::MouseButton::LeftButton)
    {
      static double elevation = 0;

      QPoint dp      = m_mouse_pos - event->pos ();
      double de      = (double) dp.y () / (double) window ()->height ();
      double dh      = (double) dp.x () / (double) window ()->width ();
      double heading = m_osg_orbit->getHeading () + dh;

      elevation -= de;
      m_osg_orbit->setElevation (elevation);
      m_osg_orbit->setHeading (heading);
      update ();
    }
  m_mouse_pos = event->pos ();
  event->accept ();
}

void
ItemView::mousePressEvent (QMouseEvent * event)
{
  m_mouse_buttons = event->buttons ();
  m_mouse_pos     = event->pos ();
  event->accept ();
}

void
ItemView::mouseReleaseEvent (QMouseEvent * event)
{
  m_mouse_buttons = event->buttons ();
  event->accept ();
}

void
ItemView::geometryChanged (const QRectF &ng, const QRectF &og)
{
  QQuickItem::geometryChanged (ng, og);

  if (ng == og)
    return;
  // force the texture to be re-initialized with correct size;
  m_size_valid = false;
}
}
