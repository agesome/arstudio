#ifndef ITEMVIEW_HPP
#define ITEMVIEW_HPP

#include <QQuickItem>
#include <QQuickWindow>
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFunctions>
#include <QSGSimpleTextureNode>
#include <QFile>
#include <QElapsedTimer>

#include <fontconfig/fontconfig.h>

#include <osgViewer/Viewer>
#include <osgViewer/Renderer>
#include <osgGA/OrbitManipulator>
#include <osgGA/FirstPersonManipulator>
#include <osg/ShapeDrawable>
#include <osg/Geometry>
#include <osg/LineWidth>
#include <osgText/Text>

#include <Scenegraph.hpp>
#include <ScenegraphAggregator.hpp>
#include <Sequence.hpp>
#include <Camera.hpp>
#include <Bitmap.hpp>

namespace arstudio {
class KeyboardCameraManipulator : public osgGA::FirstPersonManipulator
{
public:
  KeyboardCameraManipulator (int flags)
    : osgGA::FirstPersonManipulator (flags)
    , m_forward (0.0)
    , m_leftright (0.0)
    , m_updown (0.0)
  {
  }

  void
  setVelocity (const double &velocity)
  {
    _maxVelocity = velocity;
  }

protected:
  bool
  handleKeyDown (const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &)
  {
    switch (ea.getKey ())
      {
      case Qt::Key_W:
        m_forward = _maxVelocity;
        break;

      case Qt::Key_S:
        m_forward = -_maxVelocity;
        break;

      case Qt::Key_A:
        m_leftright = -_maxVelocity;
        break;

      case Qt::Key_D:
        m_leftright = _maxVelocity;
        break;

      case Qt::Key_Shift:
        m_updown = _maxVelocity;
        break;

      case Qt::Key_Control:
        m_updown = -_maxVelocity;
        break;

      case Qt::Key_Space:
        home (0);

      default: return false;
      }
    return true;
  }

  bool
  handleKeyUp (const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &)
  {
    switch (ea.getKey ())
      {
      case Qt::Key_W:
      case Qt::Key_S:
        m_forward = 0.0;
        break;

      case Qt::Key_A:
      case Qt::Key_D:
        m_leftright = 0.0;
        break;

      case Qt::Key_Shift:
      case Qt::Key_Control:
        m_updown = 0.0;
        break;

      default: return false;
      }
    return true;
  }

  bool
  handleFrame (const osgGA::GUIEventAdapter &, osgGA::GUIActionAdapter &)
  {
    moveForward (m_forward);
    moveRight (m_leftright);
    moveUp (m_updown);
    return true;
  }

private:
  double m_forward;
  double m_leftright;
  double m_updown;
};

class ItemView : public QQuickItem
{
  Q_OBJECT
  Q_PROPERTY (arstudio::Scenegraph * scenegraph READ scenegraph CONSTANT)
  Q_PROPERTY (bool show_camera_path READ show_camera_path
              WRITE set_show_camera_path
              NOTIFY show_camera_path_changed)
  Q_PROPERTY (bool show_item_positions READ show_item_positions
              WRITE set_show_item_positions
              NOTIFY show_item_positions_changed)
  Q_PROPERTY (bool first_person_mode READ first_person_mode
              WRITE set_first_person_mode
              NOTIFY first_person_mode_changed)
public:
  ItemView (QQuickItem * parent = nullptr);
  ~ItemView ();

  arstudio::Scenegraph * scenegraph ();

  inline bool
  show_camera_path ()
  {
    return m_show_camera_path;
  }

  inline void
  set_show_camera_path (bool v)
  {
    m_show_camera_path = v;
    show_camera_path_changed ();
    if (m_osg_initialized)
      update_scene ();
  }

  inline bool
  show_item_positions ()
  {
    return m_show_item_positions;
  }

  inline void
  set_show_item_positions (bool v)
  {
    m_show_item_positions = v;
    show_item_positions_changed ();
    if (m_osg_initialized)
      update_scene ();
  }

  inline bool
  first_person_mode ()
  {
    return m_first_person_mode;
  }
  inline void
  set_first_person_mode (bool v)
  {
    m_first_person_mode = v;
    first_person_mode_changed ();
    update ();
  }

protected:
  QSGNode * updatePaintNode (QSGNode *, UpdatePaintNodeData *);
  void wheelEvent (QWheelEvent * event);
  void mouseMoveEvent (QMouseEvent * event);
  void mousePressEvent (QMouseEvent * event);
  void mouseReleaseEvent (QMouseEvent * event);
  void keyPressEvent (QKeyEvent * event);
  void keyReleaseEvent (QKeyEvent * event);
  void geometryChanged (const QRectF &new_geom, const QRectF &old_geom);

private:
  void osg_init ();

  void add_camera (const Camera::ptr camera);
  void add_camera_path (const Sequence * sequence);
  void show_bitmap (const Bitmap::ptr bitmap);

  void find_font ();
  void create_axis ();

  inline osgText::Text *
  show_text (const osg::Vec3 & pos, const QString & text)
  {
    osgText::Text * p = new osgText::Text;

    if (!m_fontpath.isNull ())
      {
        p->setFont (m_fontpath.toLocal8Bit ().data ());
        p->setFontResolution (200, 200);
      }

    p->setText (text.toStdString ());
    p->setPosition (pos);
    p->setCharacterSize (0.06);
    p->setAutoRotateToScreen (true);
    p->setColor (osg::Vec4 (0, 1, 0, 1));
    return p;
  }

  inline void
  enable_osg_context (bool enable)
  {
    static QOpenGLContext * qt_context = nullptr;

    if (enable)
      {
        qt_context = QOpenGLContext::currentContext ();
        Q_ASSERT (m_osg_opengl_ctx->makeCurrent (m_window));
      }
    else if (qt_context)
      Q_ASSERT (qt_context->makeCurrent (m_window));
  }

  Scenegraph::ptr m_scenegraph;
  int             m_current_frame;
  QImage          m_current_bitmap;

  static QString m_fontpath;

  bool m_show_camera_path;
  bool m_show_item_positions;
  bool m_first_person_mode;
  bool m_size_valid;
  bool m_osg_initialized;

  QOpenGLContext           * m_osg_opengl_ctx;
  QQuickWindow             * m_window;
  QSGSimpleTextureNode     * m_texturenode;
  QOpenGLFramebufferObject * m_fbo;
  QOpenGLFramebufferObject * m_display_fbo;

  osg::ref_ptr<osgViewer::Viewer>                 m_osg_viewer;
  osg::ref_ptr<osgGA::OrbitManipulator>           m_osg_orbit;
  osg::ref_ptr<KeyboardCameraManipulator>         m_osg_firstperson;
  osg::ref_ptr<osg::Group>                        m_osg_scene;
  osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> m_osg_window_handle;
  /*
   * we will not delete this in the destructor, should be deleted internally by
   * OSG since osg::Geode derives Referenced; TODO: verify?
   */
  osg::Geode * m_sequence_node;

private slots:
  void change_frame (int frame);
  void change_item_type ();
  void window_set (QQuickWindow * window);
  void osg_render ();
  void update_scene ();

signals:
  void show_camera_path_changed ();
  void show_item_positions_changed ();
  void first_person_mode_changed ();
};
}

#endif // ITEMVIEW_HPP
