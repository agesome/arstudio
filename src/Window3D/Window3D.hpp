#ifndef SCENE3D_H
#define SCENE3D_H

#include <QGLWidget>
#include <QtGui>

#include <GL/glu.h>

#include <stdexcept>

#include <Sequence.hpp>
#include <Point3D.hpp>
#include <Camera.hpp>
#include <Scenegraph.hpp>
#include <PointCloud.hpp>

using namespace Workspace;

class Window3D : public QGLWidget
{
	Q_OBJECT
private:
	Scenegraph::ptr sg;
	int currNframe = 1;

	GLfloat xRot;
	GLfloat yRot;
	GLfloat zRot;
	GLfloat zTra;
	GLfloat nSca;

	QPoint ptrMousePosition;

	void defaultScene ();
	void drawSceneElements ();
	void drawAxis ();
	void drawCam (double x, double y, double z, double a, double rx, double ry, double rz);
	void drawPoint3D (Point3d::ptr, GLfloat);
	void drawPointCloud (PointCloud::ptr);

protected:
	void initializeGL ();
	void resizeGL (int nWidth, int nHeight);
	void paintGL ();
	void mousePressEvent (QMouseEvent* pe);
	void mouseMoveEvent (QMouseEvent* pe);
	void wheelEvent (QWheelEvent* pe);
	void keyPressEvent (QKeyEvent* pe);

public slots:
	void update (int);
	void update (void);
public:
	Window3D(Scenegraph::ptr, QWidget *parent = 0);
};
#endif
