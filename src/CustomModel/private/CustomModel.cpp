#include <CustomModel.hpp>

namespace arstudio {
CustomModel::CustomModel (QObject * parent)
  : QQuickItem3D (parent),
  m_scale (0.1),
  m_effect (nullptr),
  m_mesh (new QQuickMesh (this)),
  m_rotation_x (new QGraphicsRotation3D (this)),
  m_rotation_y (new QGraphicsRotation3D (this)),
  m_rotation_z (new QGraphicsRotation3D (this)),
  m_filename ("invalid")
{
  m_rotation_x->setAxis (QVector3D (1, 0, 0));
  m_rotation_y->setAxis (QVector3D (0, 1, 0));
  m_rotation_z->setAxis (QVector3D (0, 0, 1));

  QQmlListProperty<QQuickQGraphicsTransform3D> p = transform ();
  p.append (&p, m_rotation_x);
  p.append (&p, m_rotation_y);
  p.append (&p, m_rotation_z);

  setMesh (m_mesh);
  setObjectName ("vp_ignore");
  setScale (m_scale);
}

QVector3D
CustomModel::rotation (void)
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

QQuickEffect *
CustomModel::effect (void)
{
  return m_effect;
}

void
CustomModel::set_effect (QQuickEffect * e)
{
  m_effect = e;
  e->setParent (this);
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

  m_mesh->setSource (s);
  m_filename = i.fileName ();
  filename_changed ();
}

QString
CustomModel::filename (void)
{
  return m_filename;
}

qreal
CustomModel::scale ()
{
  return m_scale;
}

void
CustomModel::set_scale (qreal s)
{
  m_scale = s;
  setScale (s);
}
}
