#include <QtGui>
#include <math.h>
#include <window3d.hpp>
#include <timeline.hpp>
#include <QObject>
#include <list>
#include <sequence.hpp>
#include <scenegraph.hpp>
#include "camera.hpp"

const static float pi = 3.141593, k = pi / 180;

using namespace std;
using namespace boost;
using namespace Workspace;

GLfloat VertexArray[12][3];
GLfloat ColorArray[12][3];
GLubyte IndexArray[20][3];
int currNframe;

void Window3D::update (int nframe)
{
	qDebug () << nframe << "awdawdaw";

	currNframe = nframe;
	this->updateGL ();

	emit drawed ();
}

Window3D::Window3D(Scenegraph::ptr s, QWidget* parent) : QGLWidget (parent)
{
	sg = s;
	xRot = 180;
	yRot = 0;
	zRot = 180;
	zTra = 0;
	nSca = 1;


	NumberPuzzle = 1;
	CountPuzzle = 1;    // создать метод для поределения

	StepPuzzlTra = 0.01;
	StepPuzzlRot = 1.0;
	ShiftF = 0;
	currNframe = 1;

	// ДАННЫЕ БУДУТ БРАТЬСЯ ИЗ ПОЛУЧЕННОГО Sequance
	std::ifstream fConfig ("config.txt");     // читаем с Sequense SG
	fConfig >> CountPuzzle;
	points = new QVector <RgbPoint3d>[CountPuzzle];    // объект sg.getSequence
	params = new RotTraParam[CountPuzzle];    // нужно????????????no
	for (int i = 0; i < CountPuzzle; i++)
		{
// читаем из файла и записываем в массив структур поворот-перемещение
			fConfig >> params[i].xPuzzlRot;
			fConfig >> params[i].yPuzzlRot;
			fConfig >> params[i].zPuzzlRot;
			fConfig >> params[i].xPuzzlTra;
			fConfig >> params[i].yPuzzlTra;
			fConfig >> params[i].zPuzzlTra;
		}

	for (int i = 0; i < CountPuzzle; i++)
		{
			// ?????????????????????????????????????no

			QString str = "../data/ny" + QString::number (i) + ".txt";
			std::ifstream f1 (str.toAscii ());
			qDebug () << str;
			RgbPoint3d somepoint;
			f1 >> somepoint.x;
			f1 >> somepoint.y;
			f1 >> somepoint.z;
			f1 >> somepoint.r;
			f1 >> somepoint.g;
			f1 >> somepoint.b;
			while (f1.good ())
				{
					RgbPoint3d point;
					f1 >> point.x;
					f1 >> point.y;
					f1 >> point.z;
					f1 >> point.r;
					f1 >> point.g;
					f1 >> point.b;
					point.x -= somepoint.x;
					point.y -= somepoint.y;
					point.z -= somepoint.z;
					points[i].push_back (point);
				}
		}
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Window3D::initializeGL ()
{
// инициализация GL
	qglClearColor (Qt::white);
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

void Window3D::resizeGL (int nWidth, int nHeight)
{
	glMatrixMode (GL_PROJECTION);     // команды отн. к проекту
	glLoadIdentity ();


	GLfloat ratio = (GLfloat) nHeight / (GLfloat) nWidth;

	if (nWidth >= nHeight)
		glOrtho (-1.0 / ratio, 1.0 / ratio, -1.0, 1.0, -10.0, 1.0);
	else
		glOrtho (-1.0, 1.0, -1.0 * ratio, 1.0 * ratio, -10.0, 1.0);

	glViewport (0, 0, (GLint) nWidth, (GLint) nHeight);   // установка окон
}

void Window3D::paintGL ()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode (GL_MODELVIEW);     // команды отн. к модели
	glLoadIdentity ();

	glScalef (nSca, nSca, nSca);
	glTranslatef (0.0f, zTra, 0.0f);
	glRotatef (xRot, 1.0f, 0.0f, 0.0f);
	glRotatef (yRot, 0.0f, 1.0f, 0.0f);
	glRotatef (zRot, 0.0f, 0.0f, 1.0f);

	draw ();     // выбирает, что рисовать
	// this->show();
}

void Window3D::mousePressEvent (QMouseEvent* pe)
{
	ptrMousePosition = pe->pos ();
}

void Window3D::mouseReleaseEvent (QMouseEvent* pe)
{
}

void Window3D::mouseMoveEvent (QMouseEvent* pe)
{
	xRot += 180 / nSca * (GLfloat) (pe->y () - ptrMousePosition.y ()) / height ();
	zRot += 180 / nSca * (GLfloat) (pe->x () - ptrMousePosition.x ()) / width ();

	ptrMousePosition = pe->pos ();

	updateGL ();
}

void Window3D::wheelEvent (QWheelEvent* pe)
{
	if ((pe->delta ()) > 0)
		scale_plus ();
	else if ((pe->delta ()) < 0)
		scale_minus ();

	updateGL ();
}

void Window3D::keyPressEvent (QKeyEvent* pe)
{
	qDebug () << params[NumberPuzzle].xPuzzlRot << params[NumberPuzzle].yPuzzlRot << params[NumberPuzzle].zPuzzlRot;
	qDebug () << params[NumberPuzzle].xPuzzlTra << params[NumberPuzzle].yPuzzlTra << params[NumberPuzzle].zPuzzlTra;
	switch (pe->key ())
		{
		case Qt::Key_Q:
			moveMinusX ();
			break;

		case Qt::Key_W:
			movePluseX ();
			break;

		case Qt::Key_A:
			moveMinusY ();
			break;

		case Qt::Key_S:
			movePluseY ();
			break;

		case Qt::Key_Z:
			moveMinusZ ();
			break;

		case Qt::Key_X:
			movePluseZ ();
			break;

		case Qt::Key_Shift:
			FlagShift ();
			break;

		case Qt::Key_1:
			NumberPuzzle = 0;
			break;

		case Qt::Key_2:
			NumberPuzzle = 1;
			break;

		case Qt::Key_3:
			NumberPuzzle = 2;
			break;

		case Qt::Key_4:
			NumberPuzzle = 3;
			break;

		case Qt::Key_5:
			NumberPuzzle = 4;
			break;

		case Qt::Key_Plus:
			scale_plus ();
			break;

		case Qt::Key_Equal:
			scale_plus ();
			break;

		case Qt::Key_Minus:
			scale_minus ();
			break;

		case Qt::Key_Up:
			rotate_up ();
			break;

		case Qt::Key_Down:
			rotate_down ();
			break;

		case Qt::Key_Left:
			rotate_left ();
			break;

		case Qt::Key_Right:
			rotate_right ();
			break;

		case Qt::Key_Space:
			defaultScene ();
			break;

		case Qt::Key_Escape:
			this->close ();
			break;
		}

	updateGL ();
}

void Window3D::FlagShift ()
{
	ShiftF = (ShiftF ? false : true);
}
void Window3D::moveMinusX ()
{
	if (ShiftF)
		params[NumberPuzzle].xPuzzlRot -= StepPuzzlRot;
	else
		params[NumberPuzzle].xPuzzlTra -= StepPuzzlTra;
}

void Window3D::movePluseX ()
{
	if (ShiftF)
		params[NumberPuzzle].xPuzzlRot += StepPuzzlRot;
	else
		params[NumberPuzzle].xPuzzlTra += StepPuzzlTra;
}

void Window3D::moveMinusY ()
{
	if (ShiftF)
		params[NumberPuzzle].yPuzzlRot -= StepPuzzlRot;
	else
		params[NumberPuzzle].yPuzzlTra -= StepPuzzlTra;
}

void Window3D::movePluseY ()
{
	if (ShiftF)
		params[NumberPuzzle].yPuzzlRot += StepPuzzlRot;
	else
		params[NumberPuzzle].yPuzzlTra += StepPuzzlTra;
}

void Window3D::moveMinusZ ()
{
	if (ShiftF)
		params[NumberPuzzle].zPuzzlRot -= StepPuzzlRot;
	else
		params[NumberPuzzle].zPuzzlTra -= StepPuzzlTra;
}

void Window3D::movePluseZ ()
{
	if (ShiftF)
		params[NumberPuzzle].zPuzzlRot += StepPuzzlRot;
	else
		params[NumberPuzzle].zPuzzlTra += StepPuzzlTra;
}

void Window3D::scale_plus ()
{
	nSca = nSca * 1.1;
}

void Window3D::scale_minus ()
{
	nSca = nSca / 1.1;
}

void Window3D::rotate_up ()
{
	xRot += 1.0;
}

void Window3D::rotate_down ()
{
	xRot -= 1.0;
}

void Window3D::rotate_left ()
{
	zRot += 1.0;
}

void Window3D::rotate_right ()
{
	zRot -= 1.0;
}

void Window3D::translate_down ()
{
	zTra -= 0.05;
}

void Window3D::translate_up ()
{
	zTra += 0.05;
}

void Window3D::defaultScene ()
{
	xRot = -90;
	yRot = 0;
	zRot = 0;
	zTra = 0;
	nSca = 1;
}


// ВЫБИРАЕТ КАКОЙ МЕТОД РИСОВАНИЯ ВЫБИРАТЬ
void Window3D::draw ()
{
// ОСИ
	glLineWidth (3.0f);

	glColor4f (1.00f, 0.00f, 0.00f, 1.0f);
	glBegin (GL_LINES);
	glVertex3f (1.0f, 0.0f, 0.0f);
	glVertex3f (-1.0f, 0.0f, 0.0f);
	glEnd ();

	QColor halfGreen (0, 128, 0, 255);
	qglColor (halfGreen);
	glBegin (GL_LINES);
	glVertex3f (0.0f, 1.0f, -0.0f);
	glVertex3f (0.0f, -1.0f, 0.0f);

	glColor4f (0.00f, 0.00f, 1.00f, 1.0f);
	glVertex3f (0.0f, 0.0f, 1.0f);
	glVertex3f (0.0f, 0.0f, -1.0f);
	glEnd ();

	drawCam (1, 0, 0, 0.02, 0, -90, 0);
	drawCam (0, 1, 0, 0.02, 90, 0, 0);
	drawCam (0, 0, 1, 0.02, 180, 0, 0);


	double x = 0, y = 0, z = 0, a = 0.001;
	float R = 0, G = 0, B = 0;


	Sequence::ptr temp;
	Item::ptr item;

	for (Scenegraph::list::const_iterator iter = sg->getSequences ().begin ();
	     iter != sg->getSequences ().end ();
	     iter++)
		{
			temp = *iter;

			if (temp->getItems ().find (currNframe) != temp->getItems ().end ())
				{
					const Sequence::map m = temp->getItems ();
					item = temp->getItems ().find (currNframe)->second;
					// item = m.equal_range(currNframe).first;
// item = *temp->getItems().equal_range(currNframe).first;

					switch (temp->getType ())
						{
							Point3d * p;
							Camera * c;

						case Item::CAMERA:
							c = (Camera *) (item.get ());
							drawCam (c->tx, c->ty, c->tz, 0.05, c->rx, c->ry, c->rz);
							qDebug () << currNframe << c->tx << c->ty << c->tz;
							break;

						case Item::FPVEC:
							p = (Point3d *) (item.get ());
							drawPoint3D (*p, 20);
							break;
						}
				}

			qDebug () << "type = " << temp->getType ();
		}
}


// рисуем точки из полученного Sequence
void Window3D::drawPoint3D (Point3d & p, GLfloat size)
{
	glPointSize (size);
	glTranslatef (0, 0, 0);
	glColor3f (p.r, p.g, p.b);
	glBegin (GL_POINTS);
	glVertex3d (p.x, p.y, p.z);
	glColor3f (1, 1, 1);
	glEnd ();
}


// рисуем Cam из полученного Sequence
void Window3D::drawCam (double x, double y, double z, double a, double rx, double ry, double rz)
{
	glTranslatef (x, y, z);

	glRotatef (rx, 1.0f, 0.0f, 0.0f);
	glRotatef (ry, 0.0f, 1.0f, 0.0f);
	glRotatef (rz, 0.0f, 0.0f, 1.0f);

	// glRotatef(180, 1.0f, 0.0f, 0.0f);

	glBegin (GL_QUADS);


	glColor3f (1.0f, 1.0f, 0.0f);                 // Желтый

	glVertex3d (-a, -a, a);              // Верх лево
	glVertex3d (a, -a, a);               // Верх право квадрата (Зад)
	glVertex3d (+a, a, a);               // Низ право
	glVertex3d (-a, a, a);               // Низ лево


	glEnd ();

	glBegin (GL_TRIANGLES);


	glColor3f (0.0f, 0.0f, 0.0f);                 // ч
	glVertex3d (0, 0, -2 * a);             // Низ лево
	glVertex3d (a, -a, +a);               // Верх право квадрата (Низ)
	glVertex3d (-a, -a, +a);               // Верх лево

	glColor3f (1.0f, 0.0f, 0.0f);                 // Оранжевый
	glVertex3d (0, 0, -2 * a);             // Низ лево
	glVertex3d (a, a, +a);                // Верх право квадрата (Низ)
	glVertex3d (a, -a, +a);               // Верх лево


	glColor3f (1.0f, 0.0f, 0.0f);                 // Оранжевый
	glVertex3d (0, 0, -2 * a);             // Низ лево
	glVertex3d (-a, -a, +a);               // Верх право квадрата (Низ)
	glVertex3d (-a, a, +a);               // Верх лево

	glColor3f (0.0f, 0.0f, 0.0f);                 // Оранжевый
	glVertex3d (0, 0, -2 * a);             // Низ лево
	glVertex3d (-a, a, +a);              // Верх право квадрата (Низ)
	glVertex3d (a, a, +a);               // Верх лево


	glEnd ();

	glRotatef (-rz, 0.0f, 0.0f, 1.0f);
	glRotatef (-rx, 1.0f, 0.0f, 0.0f);
	glRotatef (-ry, 0.0f, 1.0f, 0.0f);

	glTranslatef (-x, -y, -z);
}

void Window3D::drawPointCloud ()
{
	for (int i = 0; i < CountPuzzle; i++)
		{
			glTranslatef (params[i].xPuzzlTra, params[i].yPuzzlTra, params[i].zPuzzlTra);
			glRotatef (params[i].xPuzzlRot, 1.0f, 0.0f, 0.0f);
			glRotatef (params[i].yPuzzlRot, 0.0f, 1.0f, 0.0f);
			glRotatef (params[i].zPuzzlRot, 0.0f, 0.0f, 1.0f);
			glPointSize (2.5);
			for (int j = 0; j < points[i].size (); j++)
				{
					glBegin (GL_POINTS);
					glColor3f (points[i].at (j).r, points[i].at (j).g, points[i].at (j).b);
					glVertex3d (points[i].at (j).x, points[i].at (j).y, points[i].at (j).z);
					glColor3f (1, 1, 1);
					glEnd ();
				}

			glRotatef (-params[i].zPuzzlRot, 0.0f, 0.0f, 1.0f);
			glRotatef (-params[i].yPuzzlRot, 0.0f, 1.0f, 0.0f);
			glRotatef (-params[i].xPuzzlRot, 1.0f, 0.0f, 0.0f);
			glTranslatef (-params[i].xPuzzlTra, -params[i].yPuzzlTra, -params[i].zPuzzlTra);
		}
}

/*из SG получаем Sequance.
   Проверяем какой тип точек надо рисовать
   Рисуем его

   update() на newFrame(int nframe)-> вызывает draw
 */






