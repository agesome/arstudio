#ifndef SCENE3D_H
#define SCENE3D_H

#include <QGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <GL/glu.h>

#include <Sequence.hpp>
#include <Point3D.hpp>
#include <Camera.hpp>
#include <Scenegraph.hpp>
#include <PointCloud.hpp>

namespace arstudio {
/**
 *      This widget handles display of Items in 3D space.
 */

class Window3D : public QGLWidget
{
  Q_OBJECT
private:
  void default_scene (void);
  void draw_scene (void);
  void draw_axis (void);
  void draw_camera (double, double, double, double, double, double, double);
  void draw_camera_path (const Sequence::frame_map &);
  void draw_point (Point3d::ptr, GLfloat);
  void draw_pointcloud (PointCloud::ptr);

  Scenegraph::ptr scenegraph_ptr;
  int             current_frame;

  GLfloat gl_rotation_x;
  GLfloat gl_rotation_y;
  GLfloat gl_rotation_z;
  GLfloat gl_translation_z;
  GLfloat gl_scale;

  QPoint mouse_position;

  static constexpr float camera_size = 0.05;
  static constexpr float point_size  = 20;
protected:
  void initializeGL (void);
  void resizeGL (int, int);
  void paintGL (void);
  void mousePressEvent (QMouseEvent *);
  void mouseMoveEvent (QMouseEvent *);
  void wheelEvent (QWheelEvent *);
  void keyPressEvent (QKeyEvent *);

public slots:
  void update (int);
  void update (void);
public:
  Window3D (Scenegraph::ptr, QWidget * parent = nullptr);
};
}
#endif
