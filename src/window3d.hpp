#ifndef SCENE3D_H
#define SCENE3D_H

#include <QGLWidget>
#include <QFile>
#include <fstream>
#include <QDebug>
#include <QObject>
#include <QVector>
#include <GL/glu.h>
#include <map>
#include <scenegraph.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/make_shared.hpp>
#include <point3d.hpp>


using namespace std;
using namespace Workspace;

struct RgbPoint3d
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat r;
	GLfloat g;
	GLfloat b;

};

struct RotTraParam
{
	GLfloat xPuzzlRot;
	GLfloat yPuzzlRot;
	GLfloat zPuzzlRot;
	GLfloat xPuzzlTra;
	GLfloat yPuzzlTra;
	GLfloat zPuzzlTra;

};

class Window3D : public QGLWidget
{

	Q_OBJECT

signals:
	void drawed();
public slots:
	void update(int);

public:
	RotTraParam *params;
	//void drawPoint3D(double x, double y, double z);

private:

	Scenegraph * sg;
	//RgbPoint3d * rgbp;
	Point3d * rgbp;
	QVector <RgbPoint3d> *points;
	GLfloat xRot;
	GLfloat yRot;
	GLfloat zRot;
	GLfloat zTra;
	GLfloat nSca;

	GLfloat StepPuzzlTra;
	GLfloat StepPuzzlRot;
	bool ShiftF;

	QPoint ptrMousePosition;
	int NumberPuzzle;
	int CountPuzzle;
	void scale_plus();
	void scale_minus();
	void rotate_up();
	void rotate_down();
	void rotate_left();
	void rotate_right();
	void translate_down();
	void translate_up();
	void defaultScene();
	void moveMinusX();
	void movePluseX();
	void moveMinusY();
	void movePluseY();
	void moveMinusZ();
	void movePluseZ();
	void FlagShift();

	void draw();
	void drawCube(double x, double y, double z, double a);
	void drawCam(double x, double y, double z, double a, double rx, double ry, double rz );
	void drawPoint3D(Point3d &,GLfloat);
	void drawPointCloud();
protected:
	void initializeGL();
	void resizeGL(int nWidth, int nHeight);
	void paintGL();
	void mousePressEvent(QMouseEvent* pe);
	void mouseMoveEvent(QMouseEvent* pe);
	void mouseReleaseEvent(QMouseEvent* pe);
	void wheelEvent(QWheelEvent* pe);
	void keyPressEvent(QKeyEvent* pe);

public:
	Window3D(Scenegraph *, QWidget *parent=0);


};
#endif
