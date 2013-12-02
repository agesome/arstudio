#ifndef ITEMVIEW_HPP
#define ITEMVIEW_HPP

#include <QElapsedTimer>
#include <QQuickItem>
#include <QQuickWindow>
#include <QOpenGLContext>
#include <QSGSimpleTextureNode>
#include <QOpenGLFramebufferObject>
#include <QFile>

#include <fontconfig/fontconfig.h>

#include <osgViewer/Viewer>
#include <osgViewer/Renderer>
#include <osgGA/OrbitManipulator>
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
    update ();
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
    update ();
  }

signals:
  void show_camera_path_changed ();
  void show_item_positions_changed ();

protected:
  QSGNode * updatePaintNode (QSGNode *, UpdatePaintNodeData *);
  void wheelEvent (QWheelEvent * event);
  void mouseMoveEvent (QMouseEvent * event);
  void mousePressEvent (QMouseEvent * event);
  void mouseReleaseEvent (QMouseEvent * event);
  void geometryChanged (const QRectF &new_geom, const QRectF &old_geom);
private:
  void osg_init ();
  void osg_paint ();
  void update_scene ();

  void add_camera (const Camera::ptr camera);
  void add_camera_path (const Sequence * sequence);
  void show_bitmap (const Bitmap::ptr bitmap);

  void find_font ();

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

  void create_axis ();

  Scenegraph::ptr m_scenegraph;
  int             m_current_frame;
  bool            m_show_camera_path;
  bool            m_show_item_positions;
  QImage          m_current_bitmap;
  QString         m_fontpath;

  QOpenGLContext * m_osg_opengl_ctx;

  QSGSimpleTextureNode       m_texturenode;
  QOpenGLFramebufferObject * m_fbo;

  osg::ref_ptr<osgViewer::Viewer>                 m_osg_viewer;
  osg::ref_ptr<osgGA::OrbitManipulator>           m_osg_orbit;
  osg::ref_ptr<osg::Group>                        m_osg_scene;
  osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> m_osg_window_handle;
  /*
   * we will not delete this in the destructor, should be deleted internally by
   * OSG since osg::Geode derives Referenced; TODO: verify?
   */
  osg::Geode * m_sequence_node;


  bool m_size_valid;
  bool m_osg_initialized;

  Qt::MouseButtons m_mouse_buttons;
  QPoint           m_mouse_pos;
private slots:
  void change_frame (int frame);
};
}

#endif // ITEMVIEW_HPP
