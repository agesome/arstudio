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
  , m_show_item_positions (true)
  , m_qt_opengl_ctx (nullptr)
  , m_osg_opengl_ctx (nullptr)
  , m_fbo (nullptr)
  , m_osg_window_handle (nullptr)
  , m_size_valid (false)
  , m_osg_initialized (false)
{
  find_font ();

  setFlag (ItemHasContents);
  setAntialiasing (true);
  setAcceptedMouseButtons (Qt::MouseButton::LeftButton);
  m_texturenode.setFlag (QSGSimpleTextureNode::OwnedByParent, false);

  // we want to redraw when: a) there may be new sequences for current frame
  // and b) when the frame index changes
  ScenegraphAggregator::instance ()->add_scenegraph (m_scenegraph.data ());
  connect (m_scenegraph.data (), &Scenegraph::sequences_changed,
           this, &ItemView::update);
  connect (ScenegraphAggregator::instance (),
           &ScenegraphAggregator::change_frame,
           this, &ItemView::change_frame);
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

void
ItemView::osg_paint ()
{
#if DEBUG_RENDERING
  static QElapsedTimer call_timer;
  call_timer.restart ();
#endif

  // viewer/camera setup on item resize
  if (!m_size_valid)
    {
      m_osg_window_handle->resized (0, 0, width (), height ());

      osg::Camera * c = m_osg_viewer->getCamera ();

      c->setRenderingCache (nullptr);
      c->setViewport (0, 0, width (), height ());
      c->setProjectionMatrixAsPerspective (30.0f,
                                           width () / height (), 1.0f,
                                           10000.0f);

      auto r =
        static_cast<osgViewer::Renderer *> (c->getRenderer ());
      osgUtil::RenderStage * rs = r->getSceneView (0)->getRenderStage ();

      rs->setCameraRequiresSetUp (true);
      rs->setFrameBufferObject (NULL);

      if (m_fbo)
        delete m_fbo;

      QOpenGLFramebufferObjectFormat fmt;
      fmt.setAttachment (QOpenGLFramebufferObject::CombinedDepthStencil);
      fmt.setSamples (4);
      m_fbo = new QOpenGLFramebufferObject (QSize (width (), height ()), fmt);

      m_size_valid = true;
    }

  // save Qt context, make OSG context current
  m_qt_opengl_ctx = QOpenGLContext::currentContext ();
  m_qt_opengl_ctx->doneCurrent ();
  m_osg_opengl_ctx->makeCurrent (window ());

  m_fbo->bind ();

  if (!m_osg_viewer->isRealized ())
    m_osg_viewer->realize ();

  // actual OSG rendering happens here
  m_osg_viewer->frame ();

  m_fbo->release ();

  // restore Qt context
  m_osg_opengl_ctx->doneCurrent ();
  m_qt_opengl_ctx->makeCurrent (window ());

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

        case Sequence::Bitmap:
          show_bitmap (item.dynamicCast<Bitmap> ());
          break;

        default:
          qWarning ("ItemView: Unhandled sequence type!");
          break;
        }
    }

#if DEBUG_RENDERING
  qDebug ("update_scene() done in %lld us", call_timer.nsecsElapsed () / 1000);
#endif
}

void
ItemView::add_camera (const Camera::ptr camera)
{
  osg::ShapeDrawable * s;
  osg::Cone          * c;
  osg::Quat            r;

  c = new osg::Cone (qvec2osg (camera->position ()), 0.03, 0.06);

  if (m_show_item_positions)
    {
      const double x = camera->position ().x (), y = camera->position ().y (),
                   z = camera->position ().z ();
      QString p      =
        QString ("%1;%2;%3").arg (x, 0, 'g', 2).arg (y, 0, 'g', 2);
      p = p.arg (z, 0, 'g', 2);

      auto ptr = show_text (qvec2osg (camera->position ()
                                      + QVector3D (.03, .03, .03)), p);
      m_sequence_node->addDrawable (ptr);
    }

  //  FIXME: rotation conversion is probably wrong
  r = osg::Quat (camera->rotation ().x (), osg::Vec3f (1, 0, 0),
                 camera->rotation ().y (), osg::Vec3f (0, 1, 0),
                 camera->rotation ().z (), osg::Vec3f (0, 0, 1));

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

  color->push_back (osg::Vec4 (1, 1, 0, 1));

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
ItemView::show_bitmap (const Bitmap::ptr bitmap)
{
  m_current_bitmap = bitmap->get ().scaled (width (), height (),
                                            Qt::KeepAspectRatio,
                                            Qt::SmoothTransformation);
  m_size_valid = false;
}

void
ItemView::find_font ()
{
  if (!FcInit ())
    return;

  FcConfig  * config  = FcConfigGetCurrent ();
  FcPattern * pattern = nullptr, * match = nullptr;
  FcResult    result;
  FcChar8   * filename;

  pattern = FcPatternBuild (NULL,
                            FC_SPACING, FcTypeInteger, FC_MONO,
                            FC_CHAR_WIDTH, FcTypeInteger, FC_WIDTH_NORMAL,
                            FC_WEIGHT, FcTypeInteger, FC_WEIGHT_NORMAL,
                            FC_SCALABLE, FcTypeBool, FcTrue,
                            NULL);
  if (!pattern)
    goto leave;

  match = FcFontMatch (config, pattern, &result);
  FcPatternGetString (match, FC_FILE, 0, &filename);
  m_fontpath = QString (reinterpret_cast<char *> (filename));
  qDebug ("ItemView: found font: %s", filename);

leave:
  FcPatternDestroy (pattern);
  FcPatternDestroy (match);
}

void
ItemView::create_axis ()
{
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
  s->setColor (osg::Vec4 (0, 0, .9, 1.0));
  axis->addDrawable (s);
  axis->addDrawable (show_text (osg::Vec3 (0, 0, 1.1), "Z"));

  // X arrow
  cyl = new osg::Cylinder (osg::Vec3 (0.5, 0, 0), cr, 1);
  s   = new osg::ShapeDrawable (cyl);
  cyl->setRotation (osg::Quat (M_PI_2, osg::Vec3 (0, 1, 0)));
  axis->addDrawable (s);
  cone = new osg::Cone (osg::Vec3 (1, 0, 0), 2 * cr, ch);
  s    = new osg::ShapeDrawable (cone);
  cone->setRotation (osg::Quat (M_PI_2, osg::Vec3 (0, 1, 0)));
  s->setColor (osg::Vec4 (.9, .9, 0, 1.0));
  axis->addDrawable (s);
  axis->addDrawable (show_text (osg::Vec3 (1.1, 0, 0), "X"));

  // Y arrow
  cyl = new osg::Cylinder (osg::Vec3 (0, 0.5, 0), cr, 1);
  s   = new osg::ShapeDrawable (cyl);
  cyl->setRotation (osg::Quat (M_PI_2, osg::Vec3 (1, 0, 0)));
  axis->addDrawable (s);
  cone = new osg::Cone (osg::Vec3 (0, 1, 0), 2 * cr, ch);
  s    = new osg::ShapeDrawable (cone);
  cone->setRotation (osg::Quat (-M_PI_2, osg::Vec3 (1, 0, 0)));
  s->setColor (osg::Vec4 (.9, 0, 0, 1.0));
  axis->addDrawable (s);
  axis->addDrawable (show_text (osg::Vec3 (0, 1.1, 0), "Y"));

  m_osg_scene->addChild (axis);
}

void
ItemView::change_frame (int frame)
{
  m_current_frame = frame;
  update ();
}

void
ItemView::osg_init ()
{
  // create a separate OpenGL context for OSG
  m_osg_opengl_ctx = new QOpenGLContext;
  m_osg_opengl_ctx->setShareContext (QOpenGLContext::currentContext ());
  Q_ASSERT (m_osg_opengl_ctx->create ());

  m_osg_viewer = new osgViewer::Viewer;
  m_osg_viewer->setThreadingModel (osgViewer::Viewer::SingleThreaded);
  m_osg_window_handle = m_osg_viewer->setUpViewerAsEmbeddedInWindow (0, 0,
                                                                     width (),
                                                                     height ());

  osg::Light * light = m_osg_viewer->getLight ();
  light->setAmbient (osg::Vec4 (.2, .2, .2, .2));

  osg::Camera * camera = m_osg_viewer->getCamera ();
  // background color for whatever is rendered with OSG
  camera->setClearColor (osg::Vec4 (.05, .05, .05, 1.0));

  m_osg_orbit = new osgGA::OrbitManipulator;
  m_osg_viewer->setCameraManipulator (m_osg_orbit.get ());
  m_osg_orbit->setHomePosition (osg::Vec3 (3, 0, 3), osg::Vec3 (0, 0, 0),
                                osg::Vec3 (0, 0, 1));
  m_osg_viewer->home ();

  m_osg_scene = new osg::Group;
  m_osg_viewer->setSceneData (m_osg_scene.get ());

  create_axis ();
}

QSGNode *
ItemView::updatePaintNode (QSGNode *, QQuickItem::UpdatePaintNodeData *)
{
#if DEBUG_RENDERING
  static QElapsedTimer call_timer;
  call_timer.restart ();
#endif

  if (!m_osg_initialized)
    {
      osg_init ();
      m_osg_initialized = true;
    }

  update_scene ();

  if (m_scenegraph->locked_to () != Scenegraph::BITMAP)
    {
      if (!m_current_bitmap.isNull ())
        {
          // nullify
          m_current_bitmap = QImage ();
          m_size_valid     = false;
        }

      osg_paint ();
    }

  m_texturenode.setTexture (window ()->createTextureFromImage (
                              m_fbo->toImage ()));
  m_texturenode.markDirty (QSGSimpleTextureNode::DirtyForceUpdate);

#if DEBUG_RENDERING
  qDebug ("updatePaintNode() done in %lld us", call_timer.nsecsElapsed () /
          1000);
#endif

  return &m_texturenode;
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
ItemView::geometryChanged (const QRectF &new_geom, const QRectF &old_geom)
{
  QQuickItem::geometryChanged (new_geom, old_geom);

  if (new_geom == old_geom)
    return;

  m_texturenode.setRect (new_geom);
  m_size_valid = false;
}
}
