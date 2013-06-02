#ifndef IWMANAGER_HPP
#define IWMANAGER_HPP

#include <QQuickPaintedItem>
#include <QQmlEngine>

#ifndef QT_NO_DEBUG
#include <QDateTime>
#endif

#include <QQuickItem3D>
#include <QQuickMesh>
#include <QQuickEffect>
#include <QGraphicsRotation3D>

#include <line.h>
#include <spheremesh.h>
#include <capsulemesh.h>
#include <cylindermesh.h>
#include <point.h>

#include <Scenegraph.hpp>
#include <ScenegraphAggregator.hpp>
#include <Camera.hpp>

namespace arstudio {
class IWManager : public QObject
{
  Q_OBJECT
  Q_PROPERTY (arstudio::Scenegraph * scenegraph READ scenegraph)
  Q_PROPERTY (QQuickPaintedItem * viewport READ viewport WRITE set_viewport)
public:
  IWManager (QObject * parent = nullptr);

  QQuickPaintedItem * viewport (void);
  void set_viewport (QQuickPaintedItem * viewport);
  arstudio::Scenegraph * scenegraph (void);
private:
  void add_camera (Camera::ptr);
  void add_camera_path (Sequence *);

  QQuickPaintedItem * m_viewport;
  CylinderMesh      * m_camera_mesh;
  QQuickEffect      * m_camera_effect;
  SphereMesh        * m_path_mesh;
  QQuickEffect      * m_path_effect;
  QQuickEffect      * m_line_effect;
  Scenegraph::ptr     m_scenegraph;
  int                 m_current_frame;
public slots:
  void paint_frame (int);
  void repaint_frame (void);
};
}

#endif // IWMANAGER_HPP
