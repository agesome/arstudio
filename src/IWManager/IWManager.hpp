#ifndef IWMANAGER_HPP
#define IWMANAGER_HPP

#include <cmath>

#include <QQuickPaintedItem>
#include <QQmlEngine>
#include <QKeyEvent>

#ifndef QT_NO_DEBUG
#include <QDateTime>
#endif

#include <QQuickItem3D>
#include <QQuickMesh>
#include <QQuickEffect>
#include <QGraphicsRotation3D>
#include <QGLCamera>
#include <QPlane3D>
#include <QRay3D>

#include <line.h>
#include <spheremesh.h>
#include <capsulemesh.h>
#include <cylindermesh.h>
#include <point.h>

#include <Scenegraph.hpp>
#include <ScenegraphAggregator.hpp>
#include <Camera.hpp>

namespace arstudio {
/**
 * ItemWindow manager
 *
 * This class is responsible for managing the contents of QML component
 * ItemWindow. Doing that in QML was slow, hence this class was created.
 *
 * IWManager listens to Scenegraph associated with the ItemWindow and
 * displays it's contents on the associated Viewport
 */

class IWManager : public QObject
{
  Q_OBJECT
  Q_PROPERTY (arstudio::Scenegraph * scenegraph READ scenegraph)
  Q_PROPERTY (QQuickPaintedItem * viewport READ viewport WRITE set_viewport)
  Q_PROPERTY (QGLCamera * camera READ camera WRITE set_camera)
  Q_PROPERTY (bool camera_view READ camera_view WRITE set_camera_view)
  Q_PROPERTY (QVector3D camera_view_position READ camera_view_position
              NOTIFY camera_view_position_changed)
  Q_PROPERTY (qreal camera_view_distance READ camera_view_distance
              WRITE set_camera_view_distance)
public:
  IWManager (QObject * parent = nullptr);

  QQuickPaintedItem * viewport (void);
  void set_viewport (QQuickPaintedItem * viewport);
  QGLCamera * camera (void);
  void set_camera (QGLCamera *);
  bool camera_view (void);
  void set_camera_view (bool);

  QVector3D camera_view_position (void);
  qreal camera_view_distance (void);
  void set_camera_view_distance (qreal);

  arstudio::Scenegraph * scenegraph (void);
private:
  enum Axis { X, Y, Z };

  void add_camera (Camera::ptr);
  void add_camera_path (Sequence *);
  inline void add_line (const QList<QVariant> &);
  inline void add_line (const QList<QVector3D> &);
  inline void add_sphere (const QVector3D &);
  void look_from_camera (Camera::ptr);

  inline void reset_camera (const Axis axis = Z);

  QQuickPaintedItem * m_viewport;
  QGLCamera         * m_camera;
  CylinderMesh      * m_cylinder_mesh;
  QQuickEffect      * m_camera_effect;
  SphereMesh        * m_sphere_mesh;
  QQuickEffect      * m_path_effect;
  QQuickEffect      * m_line_effect;
  Scenegraph::ptr     m_scenegraph;
  int                 m_current_frame;
  bool                m_camera_view;
  qreal               m_camera_view_distance;
  QVector3D           m_camera_view_position;
public slots:
  void paint_frame (int);
  void repaint_frame (void);
  void viewport_keypress (const Qt::Key);
signals:
  void camera_view_position_changed (void);
};
}

#endif // IWMANAGER_HPP
