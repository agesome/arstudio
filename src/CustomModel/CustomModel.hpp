#ifndef CUSTOMMODEL_HPP
#define CUSTOMMODEL_HPP

#include <QQuickItem3D>
#include <QQuickMesh>
#include <QQuickEffect>
#include <QGraphicsRotation3D>
#include <QVector3D>
#include <QUrl>
#include <QString>
#include <QFileInfo>

namespace arstudio {
/**
 * @brief Simple wrapper class to allow easy model handling
 */
class CustomModel : public QQuickItem3D
{
  Q_OBJECT
  Q_PROPERTY (QVector3D rotation READ rotation WRITE set_rotation)
public:
  CustomModel (QObject * parent = nullptr);

  QVector3D rotation ();
  void set_rotation (const QVector3D &);

  void set_source (const QUrl &);
private:
  QVector3D             m_rotation;
  QGraphicsRotation3D * m_rotation_x;
  QGraphicsRotation3D * m_rotation_y;
  QGraphicsRotation3D * m_rotation_z;
};
}

#endif // CUSTOMMODEL_HPP
