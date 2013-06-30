#include <CustomModel.hpp>

namespace arstudio {
CustomModel::CustomModel (QObject * parent)
  : QQuickItem3D (parent),
  m_rotation_x (new QGraphicsRotation3D (this)),
  m_rotation_y (new QGraphicsRotation3D (this)),
  m_rotation_z (new QGraphicsRotation3D (this))
{
  m_rotation_x->setAxis (QVector3D (1, 0, 0));
  m_rotation_y->setAxis (QVector3D (0, 1, 0));
  m_rotation_z->setAxis (QVector3D (0, 0, 1));

  QQmlListProperty<QQuickQGraphicsTransform3D> p = transform ();
  p.append (&p, m_rotation_x);
  p.append (&p, m_rotation_y);
  p.append (&p, m_rotation_z);

  setObjectName ("vp_ignore");
  setScale (0.1);
}

QVector3D
CustomModel::rotation ()
{
  return m_rotation;
}

void
CustomModel::set_rotation (const QVector3D & r)
{
  m_rotation = r;
  m_rotation_x->setAngle (r.x ());
  m_rotation_y->setAngle (r.y ());
  m_rotation_z->setAngle (r.z ());
}

void
CustomModel::set_source (const QUrl & s)
{
  QFileInfo i (s.path ());

  if (!i.exists ())
    {
      qWarning () << QString ("%0: bad source path").arg (__FUNCTION__);
      return;
    }

  if (mesh ())
    {
      auto m = mesh ();
      setMesh (nullptr);
      delete m;
    }

  setMesh (new QQuickMesh (this));
  mesh ()->setSource (s);
}
}
