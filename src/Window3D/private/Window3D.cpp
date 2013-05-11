#include <Window3D.hpp>

namespace arstudio {
Window3D::Window3D (Scenegraph::ptr s, QWidget * parent) : QGLWidget (parent)
{
  this->scenegraph_ptr        = s;
  this->current_frame         = 1;
  this->last_successful_frame = -1;

  setFocusPolicy (Qt::ClickFocus);
  default_scene ();
}

void
Window3D::update (int nframe)
{
  current_frame = nframe;
  if (current_frame == 1)
    last_successful_frame = -1;
  updateGL ();
}

void
Window3D::update_current (void)
{
  update (current_frame);
}

void
Window3D::initializeGL (void)
{
  // инициализация GL
  qglClearColor (Qt::black);
  glFrontFace (GL_CW);
  // для работы с буффером глубины 3 стр
  glClearDepth (1.0f);
  glEnable (GL_DEPTH_TEST);
  glDepthFunc (GL_LEQUAL);
  glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

  glShadeModel (GL_FLAT);

  glEnableClientState (GL_VERTEX_ARRAY);
  glEnableClientState (GL_COLOR_ARRAY);
}

void
Window3D::resizeGL (int width, int height)
{
  glMatrixMode (GL_PROJECTION); // команды отн. к проекту
  glLoadIdentity ();

  GLfloat ratio = (GLfloat) height / (GLfloat) width;

  if (width >= height)
    glOrtho (-1.0 / ratio, 1.0 / ratio, -1.0, 1.0, -10.0, 1.0);
  else
    glOrtho (-1.0, 1.0, -1.0 * ratio, 1.0 * ratio, -10.0, 1.0);

  glViewport (0, 0, (GLint) width, (GLint) height); // установка окон
}

void
Window3D::paintGL (void)
{
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode (GL_MODELVIEW); // команды отн. к модели
  glLoadIdentity ();

  glScalef (gl_scale, gl_scale, gl_scale);

  glRotatef (gl_rotation_x, 1.0f, 0.0f, 0.0f);
  glRotatef (gl_rotation_y, 0.0f, 1.0f, 0.0f);
  glRotatef (gl_rotation_z, 0.0f, 0.0f, 1.0f);

  draw_scene ();
}

void
Window3D::mousePressEvent (QMouseEvent * pe)
{
  mouse_position = pe->pos ();
}

void
Window3D::mouseMoveEvent (QMouseEvent * pe)
{
  gl_rotation_x += 180 / gl_scale *
                   (GLfloat) (pe->y () - mouse_position.y ()) / height ();
  gl_rotation_z += 180 / gl_scale *
                   (GLfloat) (pe->x () - mouse_position.x ()) / width ();

  mouse_position = pe->pos ();

  updateGL ();
}

void
Window3D::wheelEvent (QWheelEvent * pe)
{
  if (pe->delta () > 0)
    gl_scale = gl_scale * 1.1;
  else if (pe->delta () < 0)
    gl_scale = gl_scale / 1.1;

  updateGL ();
}

void
Window3D::keyPressEvent (QKeyEvent * pe)
{
  switch (pe->key ())
    {
    case Qt::Key_Plus:
    case Qt::Key_Equal:
      gl_scale = gl_scale * 1.1;
      break;

    case Qt::Key_Minus:
      gl_scale = gl_scale / 1.1;
      break;

    case Qt::Key_Up:
      gl_rotation_x += 1.0;
      break;

    case Qt::Key_Down:
      gl_rotation_x -= 1.0;
      break;

    case Qt::Key_Left:
      gl_rotation_z += 1.0;
      break;

    case Qt::Key_Right:
      gl_rotation_z -= 1.0;
      break;

    case Qt::Key_Space:
      default_scene ();
      break;
    }

  updateGL ();
}

void
Window3D::default_scene (void)
{
  gl_rotation_x = 180;
  gl_rotation_y = 0;
  gl_rotation_z = 180;
  gl_scale      = 1;
}

void
Window3D::draw_scene (void)
{
  draw_axis ();

  Item::ptr   item;
  Camera::ptr camera;
  Item::type  item_type;
  bool        updated = false;

  for (const auto seq : scenegraph_ptr->sequences ())
    {
      item_type = seq->type ();
      // skip bitmaps
      if (item_type == Item::BITMAP)
        continue;

      const Sequence::frame_map & items = seq->items ();

      auto it = items.find (current_frame);
      if (it == items.end ())
        continue;
      item    = it->second;
      updated = true;

      switch (item_type)
        {
        case Item::CAMERA:
          draw_camera_path (items);
          camera = Item::ptr_cast_to<Camera> (item);
          draw_camera (camera->tx, camera->ty, camera->tz, camera_size,
                       camera->rx, camera->ry, camera->rz);
          break;

        case Item::POINT3D:
          draw_point (Item::ptr_cast_to<Point3d> (item), point_size);
          break;

        case Item::PCLOUD:
          draw_pointcloud (Item::ptr_cast_to<PointCloud> (item));
          break;

        default: break;
        }
    }
  if (!updated)
    {
      if (last_successful_frame == -1)
        return;
      int cf = current_frame;
      update (last_successful_frame);
      current_frame = cf;
    }
  else
    last_successful_frame = current_frame;
}

void
Window3D::draw_axis (void)
{
  glLineWidth (2.0f);

  glBegin (GL_LINES);
  // x (red)
  glColor4f (1.00f, 0.00f, 0.00f, 1.0f);
  glVertex3f (1.0f, 0.0f, 0.0f);
  glVertex3f (-1.0f, 0.0f, 0.0f);

  // y (green)
  glColor4f (0.0f, 1.0f, 0.0f, 1.0f);
  glVertex3f (0.0f, 1.0f, -0.0f);
  glVertex3f (0.0f, -1.0f, 0.0f);

  // z (blue)
  glColor4f (0.00f, 0.00f, 1.00f, 1.0f);
  glVertex3f (0.0f, 0.0f, 1.0f);
  glVertex3f (0.0f, 0.0f, -1.0f);
  glEnd ();

  draw_camera (1, 0, 0, 0.02, 0, -90, 0);
  draw_camera (0, 1, 0, 0.02, 90, 0, 0);
  draw_camera (0, 0, 1, 0.02, 180, 0, 0);
}

void
Window3D::draw_point (Point3d::ptr p, GLfloat size)
{
  glPointSize (size);
  glTranslatef (0, 0, 0);

  glBegin (GL_POINTS);
  glColor3f (p->r, p->g, p->b);
  glVertex3d (p->x, p->y, p->z);
  glColor3f (1, 1, 1);
  glEnd ();
}

void
Window3D::draw_pointcloud (PointCloud::ptr pc)
{
  for (auto p : pc->cloud)
    draw_point (p, point_size);
}

void
Window3D::draw_camera (double x, double y, double z, double a, double rx,
                       double ry, double rz)
{
  glPushMatrix (); // сохранить текущюю систему координат

  // повернуть и переместить систему координат
  glTranslatef (x, y, z);
  glRotatef (rx, 1.0f, 0.0f, 0.0f);
  glRotatef (ry, 0.0f, 1.0f, 0.0f);
  glRotatef (rz, 0.0f, 0.0f, 1.0f);

  // основание пирамиды
  glBegin (GL_QUADS);
  glColor3f (1.0f, 1.0f, 0.0f);              // Желтый
  glVertex3d (-a, -a, a);                    // Верх лево
  glVertex3d (a, -a, a);                     // Верх право квадрата (Зад)
  glVertex3d (+a, a, a);                     // Низ право
  glVertex3d (-a, a, a);                     // Низ лево
  glEnd ();

  glBegin (GL_TRIANGLES);
  glColor3f (1.0f, 1.0f, 1.0f);             // Белый
  glVertex3d (0, 0, -2 * a);                // Низ лево
  glVertex3d (a, -a, +a);                   // Верх право квадрата (Низ)
  glVertex3d (-a, -a, +a);                  // Верх лево

  glColor3f (1.0f, 0.0f, 0.0f);             // Красный
  glVertex3d (0, 0, -2 * a);                // Низ лево
  glVertex3d (a, a, +a);                    // Верх право квадрата (Низ)
  glVertex3d (a, -a, +a);                   // Верх лево

  glColor3f (0.0f, 0.0f, 1.0f);             // Синий
  glVertex3d (0, 0, -2 * a);                // Низ лево
  glVertex3d (-a, -a, +a);                  // Верх право квадрата (Низ)
  glVertex3d (-a, a, +a);                   // Верх лево

  glColor3f (0.0f, 1.0f, 0.0f);             // Зеленый
  glVertex3d (0, 0, -2 * a);                // Низ лево
  glVertex3d (-a, a, +a);                   // Верх право квадрата (Низ)
  glVertex3d (a, a, +a);                    // Верх лево
  glEnd ();

  glPopMatrix (); // Восстановление исходную систему координат
}

void
Window3D::draw_camera_path (const Sequence::frame_map & fm)
{
  Camera::ptr camera;
  Camera::ptr prev;

  for (auto item : fm)
    {
      camera = Item::ptr_cast_to<Camera> (item.second);
      if (!prev)
        {
          prev = camera;
          continue;
        }

      glBegin (GL_LINES);
      glLineWidth (2.0f);
      glColor4f (1.00f, 0.00f, 0.00f, 1.0f);
      glVertex3f (camera->tx, camera->ty, camera->tz);
      glVertex3f (prev->tx, prev->ty, prev->tz);
      glEnd ();
      draw_camera (camera->tx, camera->ty, camera->tz, camera_size / 6,
                   camera->rx, camera->ry, camera->rz);
      prev = camera;
    }
}
}
