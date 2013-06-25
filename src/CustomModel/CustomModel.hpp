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
class CustomModel : public QQuickItem3D
{
  Q_OBJECT
  Q_PROPERTY (QVector3D rotation READ rotation WRITE set_rotation)
  Q_PROPERTY (qreal scale READ scale WRITE set_scale)
  Q_PROPERTY (QQuickEffect * effect READ effect WRITE set_effect)
  Q_PROPERTY (QString filename READ filename NOTIFY filename_changed)
public:
  CustomModel (QObject * parent = nullptr);

  QVector3D rotation ();
  void set_rotation (const QVector3D &);
  QQuickEffect * effect ();
  void set_effect (QQuickEffect *);

  void set_source (const QUrl &);

  QString filename ();

  qreal scale ();
  void set_scale (qreal);
private:
  QVector3D             m_rotation;
  qreal                 m_scale;
  QQuickEffect        * m_effect;
  QQuickMesh          * m_mesh;
  QGraphicsRotation3D * m_rotation_x;
  QGraphicsRotation3D * m_rotation_y;
  QGraphicsRotation3D * m_rotation_z;
  QString               m_filename;
signals:
  void filename_changed ();
};
}

#endif // CUSTOMMODEL_HPP
