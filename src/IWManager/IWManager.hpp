#ifndef IWMANAGER_HPP
#define IWMANAGER_HPP

#include <QQuickItem3D>
#include <QQuickMesh>
#include <QQuickEffect>
#include <QQuickPaintedItem>
#include <QQmlEngine>
#include <QGraphicsRotation3D>

#include <line.h>
#include <spheremesh.h>
#include <capsulemesh.h>
#include <cylindermesh.h>
#include <point.h>

#include <Scenegraph.hpp>
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
  Scenegraph::ptr     m_scenegraph;
  int                 m_current_frame;
public slots:
  void paint_frame (int);
};
}

#endif // IWMANAGER_HPP
