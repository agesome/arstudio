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

QString ItemView::m_fontpath;

ItemView::ItemView (QQuickItem * parent)
  : QQuickItem (parent)
  , m_scenegraph (Scenegraph::make ())
  , m_current_frame (1)
  , m_show_camera_path (true)
  , m_show_item_positions (true)
  , m_first_person_mode (false)
  , m_size_valid (false)
  , m_osg_initialized (false)
  , m_osg_opengl_ctx (nullptr)
  , m_window (nullptr)
  , m_texturenode (nullptr)
  , m_fbo (nullptr)
  , m_display_fbo (nullptr)
  , m_sequence_node (nullptr)
{
  if (m_fontpath.isNull ())
    find_font ();

  setFlag (ItemHasContents);
  setAcceptedMouseButtons (Qt::MouseButton::LeftButton);

  // we want to redraw when: a) there may be new sequences for current frame
  // and b) when the frame index changes
  ScenegraphAggregator::instance ()->add_scenegraph (m_scenegraph.data ());

  connect (m_scenegraph.data (), &Scenegraph::sequences_changed,
           this, &ItemView::update_scene, Qt::QueuedConnection);

  connect (m_scenegraph.data (), &Scenegraph::locked_to_changed,
           this, &ItemView::change_item_type);

  connect (ScenegraphAggregator::instance (),
           &ScenegraphAggregator::change_frame,
           this, &ItemView::change_frame, Qt::QueuedConnection);

  connect (this, &ItemView::windowChanged, this, &ItemView::window_set,
           Qt::DirectConnection);
}

ItemView::~ItemView ()
{
  if (ScenegraphAggregator::instance ())
    ScenegraphAggregator::instance ()->remove_scenegraph (m_scenegraph.data ());





  if (m_fbo)
    delete m_fbo;
  if (m_display_fbo)
    delete m_display_fbo;
  if (m_osg_opengl_ctx)
    delete m_osg_opengl_ctx;
}

Scenegraph *
ItemView::scenegraph ()
{
  QQmlEngine::setObjectOwnership (m_scenegraph.data (),
                                  QQmlEngine::CppOwnership);

  return m_scenegraph.data ();
}

void
ItemView::osg_render ()
{
#if DEBUG_RENDERING
  static QElapsedTimer call_timer;
  call_timer.restart ();
#endif

  //check camera mode
  if (m_osg_viewer->getCameraManipulator () == m_osg_orbit.get () &&
      m_first_person_mode)
    {
      osg::Matrixd mat = m_osg_orbit->getMatrix ();
      m_osg_firstperson->setByMatrix (mat);
      m_osg_viewer->setCameraManipulator (m_osg_firstperson.get ());
    }
  if (m_osg_viewer->getCameraManipulator () == m_osg_firstperson.get () &&
      !m_first_person_mode)
    m_osg_viewer->setCameraManipulator (m_osg_orbit.get ());

  enable_osg_context (true);
  if (!m_fbo->bind ())
    qFatal ("Failed to bind FBO!");

  // viewer/camera setup on item resize
  if (!m_size_valid)
    {
      osg::Camera * c = m_osg_viewer->getCamera ();

      m_osg_window_handle->resized (0, 0, width (), height ());
      c->setViewport (0, 0, width (), height ());
      c->setProjectionMatrixAsPerspective (30.0f,
                                           width () / height (), 1.0f,
                                           10000.0f);
      m_size_valid = true;
    }

  // actual OSG rendering happens here
  m_osg_viewer->frame ();

  if (!m_fbo->bindDefault ())
    qFatal ("Failed to release FBO!");
  enable_osg_context (false);

  // need to blit to non-antialiased FBO for this to work
  QOpenGLFramebufferObject::blitFramebuffer (m_display_fbo, m_fbo);

#if DEBUG_RENDERING
  qDebug ("paint() done in %lld us", call_timer.nsecsElapsed () / 1000);
#endif
}

void
ItemView::osg_init ()
{
  // create a separate OpenGL context for OSG
  m_osg_opengl_ctx = new QOpenGLContext;
  m_osg_opengl_ctx->setShareContext (QOpenGLContext::currentContext ());
  m_osg_opengl_ctx->setFormat (window ()->format ());
  if (!m_osg_opengl_ctx->create ())
    qFatal ("Failed to create OSG OpenGL context!");

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
  constexpr int flags = KeyboardCameraManipulator::UPDATE_MODEL_SIZE;
  m_osg_firstperson = new KeyboardCameraManipulator (flags);
  m_osg_firstperson->setVelocity (0.05);

  m_osg_firstperson->setHomePosition (osg::Vec3 (3, 0, 3), osg::Vec3 (0, 0, 0),
                                      osg::Vec3 (0, 0, 1));

  m_osg_orbit->setHomePosition (osg::Vec3 (3, 0, 3), osg::Vec3 (0, 0, 0),
                                osg::Vec3 (0, 0, 1));
  // stop spinning when releasing LMB while moving the mouse
  m_osg_orbit->setAllowThrow (false);

  m_osg_viewer->setCameraManipulator (m_osg_orbit.get ());
  m_osg_viewer->home ();

  m_osg_scene     = new osg::Group;
  m_sequence_node = new osg::Geode;
  m_osg_scene->addChild (m_sequence_node);
  m_osg_viewer->setSceneData (m_osg_scene.get ());

  create_axis ();

  // check to see if multisampling is supported
  //FIXME: failed to bind FBO when uncommented - on noveau
  QOpenGLFunctions f (QOpenGLContext::currentContext ());
  if (f.hasOpenGLFeature (QOpenGLFunctions::Multisample))
    setAntialiasing (true);
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

  if (m_texturenode == nullptr)
    {
      m_texturenode = new QSGSimpleTextureNode;
      m_texturenode->setFiltering (QSGTexture::Linear);
    }

  QSGTexture * p = nullptr;

  if (m_scenegraph->locked_to () == Scenegraph::BITMAP)
    {
      QImage scaled = m_current_bitmap.scaled (width (), height (),
                                               Qt::KeepAspectRatio,
                                               Qt::SmoothTransformation);
      p = window ()->createTextureFromImage (scaled);
      m_texturenode->setRect (0, 0, scaled.width (), scaled.height ());
      m_texturenode->markDirty (QSGSimpleTextureNode::DirtyGeometry);
    }
  else if (!m_size_valid)
    {
      if (m_fbo)
        delete m_fbo;

      QOpenGLFramebufferObjectFormat format;
      format.setAttachment (QOpenGLFramebufferObject::CombinedDepthStencil);

      enable_osg_context (true);
      if (antialiasing ())
        format.setSamples (16);
      m_fbo = new QOpenGLFramebufferObject (boundingRect ().size ().toSize (),
                                            format);
      enable_osg_context (false);

      /*
       * QOpenGLFramebufferObject documentation suggests that if we want to
       * use it as a texture and have multisampling, we must use a second FB
       * without multisampling and blit to it (done in rendering code)
       */
      format.setSamples (0);
      m_display_fbo = new QOpenGLFramebufferObject (m_fbo->size (), format);

      p = m_window->createTextureFromId (m_display_fbo->texture (),
                                         m_display_fbo->size ());

      /*
       * Rendered texture appers to be upside down, so we invert it
       */
      QRectF rect = boundingRect ();
      rect.setTop (rect.bottom ());
      rect.setBottom (boundingRect ().top ());
      m_texturenode->setRect (rect);

      m_texturenode->markDirty (QSGSimpleTextureNode::DirtyGeometry);
    }

  if (p)
    m_texturenode->setTexture (p);
  m_texturenode->markDirty (QSGSimpleTextureNode::DirtyMaterial);

#if DEBUG_RENDERING
  qDebug ("updatePaintNode() done in %lld us", call_timer.nsecsElapsed () /
          1000);
#endif

  return m_texturenode;
}

void
ItemView::update_scene ()
{
#if DEBUG_RENDERING
  static QElapsedTimer call_timer;
  call_timer.restart ();
#endif

  m_sequence_node->removeDrawables (0, m_sequence_node->getNumDrawables ());

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
  update ();
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
  m_current_bitmap = bitmap->get ();
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
  update_scene ();
}

void
ItemView::change_item_type ()
{
  // force texture node to be re-assigned to m_fbo->texture();
  m_size_valid = false;
  update_scene ();
}

void
ItemView::window_set (QQuickWindow * window)
{
  if (window == nullptr)
    return;

  m_window = window;
  connect (window, &QQuickWindow::beforeRendering, this, &ItemView::osg_render,
           Qt::DirectConnection);
}

void
ItemView::wheelEvent (QWheelEvent * event)
{
  osgGA::GUIEventAdapter::ScrollingMotion motion;

  motion = event->angleDelta ().y () < 0 ? osgGA::GUIEventAdapter::SCROLL_UP :
           osgGA::GUIEventAdapter::SCROLL_DOWN;

  m_osg_window_handle->getEventQueue ()->mouseScroll (motion);
  event->accept ();
  update ();
}

void
ItemView::mouseMoveEvent (QMouseEvent * event)
{
  m_osg_window_handle->getEventQueue ()->mouseMotion (event->x (), event->y ());
  event->accept ();
  update ();
}

void
ItemView::mousePressEvent (QMouseEvent * event)
{
  auto eq = m_osg_window_handle->getEventQueue ();

  /*
   * we only accept left mouse button for now
   */
  eq->mouseButtonPress (event->x (), event->y (),
                        osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON);

  event->accept ();
  update ();
}

void
ItemView::mouseReleaseEvent (QMouseEvent * event)
{
  auto eq = m_osg_window_handle->getEventQueue ();

  eq->mouseButtonRelease (event->x (), event->y (),
                          osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON);
  event->accept ();
}

void
ItemView::keyPressEvent (QKeyEvent * event)
{
  /*
   * this is later handled in KeyboardCameraManipulator
   */
  m_osg_window_handle->getEventQueue ()->keyPress (event->key ());
  event->accept ();
  update ();
}

void
ItemView::keyReleaseEvent (QKeyEvent * event)
{
  m_osg_window_handle->getEventQueue ()->keyRelease (event->key ());
  event->accept ();
  update ();
}

void
ItemView::geometryChanged (const QRectF &new_geom, const QRectF &old_geom)
{
  QQuickItem::geometryChanged (new_geom, old_geom);

  if (new_geom.size () != old_geom.size ())
    {
      m_size_valid = false;
      update ();
    }
}
}
