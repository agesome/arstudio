#ifndef ITEMVIEW_HPP
#define ITEMVIEW_HPP

#include <QElapsedTimer>
#include <QQuickItem>
#include <QQuickWindow>
#include <QOpenGLContext>
#include <QSGTextureMaterial>
#include <QSGOpaqueTextureMaterial>
#include <QSGSimpleTextureNode>
#include <QFile>

#include <fontconfig/fontconfig.h>

#include <osgViewer/Viewer>
#include <osgViewer/Renderer>
#include <osgGA/OrbitManipulator>
#include <osg/Texture2D>
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
  void geometryChanged (const QRectF &ng, const QRectF &og);
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
        p->setFontResolution (100, 100);
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
  osg::Geode    * m_sequence_node;
  int             m_current_frame;
  bool            m_show_camera_path;
  bool            m_show_item_positions;
  QImage          m_current_bitmap;
  QString         m_fontpath;

  QOpenGLContext * m_qt_opengl_ctx;
  QOpenGLContext * m_osg_opengl_ctx;

  QSGGeometryNode          m_geometry_node;
  QSGTexture             * m_qt_texture;
  QSGTextureMaterial       m_qt_tex_material;
  QSGOpaqueTextureMaterial m_qt_opaque_material;
  QSGGeometry              m_qt_geometry;

  osg::ref_ptr<osgViewer::Viewer>       m_osg_viewer;
  osg::ref_ptr<osg::Texture2D>          m_osg_texture;
  osg::ref_ptr<osgGA::OrbitManipulator> m_osg_orbit;
  osg::ref_ptr<osg::Group>              m_osg_scene;
  osg::Texture::TextureObject         * m_osg_texture_object;
  osgViewer::GraphicsWindowEmbedded   * m_osg_window_handle;

  bool m_size_valid;
  bool m_osg_initialized;

  Qt::MouseButtons m_mouse_buttons;
  QPoint           m_mouse_pos;
private slots:
  void change_frame (int frame);
};
}

#endif // ITEMVIEW_HPP
