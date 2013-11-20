#ifndef ITEMVIEW_HPP
#define ITEMVIEW_HPP

#include <QElapsedTimer>
#include <QQuickItem>
#include <QQuickWindow>
#include <QOpenGLContext>
#include <QSGTextureMaterial>
#include <QSGOpaqueTextureMaterial>
#include <QSGSimpleTextureNode>

#include <osgViewer/Viewer>
#include <osgViewer/Renderer>
#include <osgGA/OrbitManipulator>
#include <osg/Texture2D>
#include <osg/ShapeDrawable>
#include <osg/Geometry>
#include <osg/LineWidth>

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
public:
  ItemView (QQuickItem * parent = nullptr);
  ~ItemView ();

  arstudio::Scenegraph * scenegraph ();

  bool show_camera_path ();
  void set_show_camera_path (bool v);

signals:
  void show_camera_path_changed ();

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

  void create_axis ();

  Scenegraph::ptr m_scenegraph;
  osg::Geode    * m_sequence_node;
  int             m_current_frame;
  bool            m_show_camera_path;
  QImage          m_current_bitmap;

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

  bool m_size_valid;
  bool m_osg_initialized;

  Qt::MouseButtons m_mouse_buttons;
  QPoint           m_mouse_pos;
private slots:
  void change_frame (int frame);
};
}

#endif // ITEMVIEW_HPP
