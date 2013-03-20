#include <ui/window3d.hpp>

using namespace std;
using namespace boost;
using namespace Workspace;

void Window3D::update (int nframe)
{
	currNframe = nframe;
	this->updateGL ();
}

Window3D::Window3D(Scenegraph::ptr s, QWidget* parent) : QGLWidget (parent)
{
	sg = s;

	defaultScene ();

	currNframe = 1;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Window3D::initializeGL ()
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

	drawSceneElements ();     // выбирает, что рисовать
}

void Window3D::mousePressEvent (QMouseEvent* pe)
{
	ptrMousePosition = pe->pos ();
}

void Window3D::mouseReleaseEvent (QMouseEvent *)
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
	switch (pe->key ())
		{
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
		}

	updateGL ();
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
    xRot = 180;
    yRot = 0;
    zRot = 180;
    zTra = 0;
	nSca = 1;
}


void Window3D::drawSceneElements ()
{
	drawAxis ();

	float cam_size = 0.05;
	float point_size = 20;

	Sequence::ptr temp;
	Item::ptr item;
	Camera::ptr c;
	for (Scenegraph::list::const_iterator iter = sg->getSequences ().begin ();
	     iter != sg->getSequences ().end ();
	     iter++)
		{
			temp = *iter;

			if (temp->getItems ().find (currNframe) != temp->getItems ().end ())
				{
					const Sequence::map m = temp->getItems ();
					item = temp->getItems ().find (currNframe)->second;

					switch (temp->getType ())
						{
						case Item::CAMERA:
						{
							Camera::ptr prev;
							for (auto it : temp->getItems ())
								{
									c = Item::ptr_cast_to<Camera> (it.second);
									if (!prev)
										{
											prev = c;
											continue;
										}

                                    glBegin (GL_LINES);
                                        glLineWidth (2.0f);
                                        glColor4f (1.00f, 0.00f, 0.00f, 1.0f);
                                        glVertex3f (c->tx, c->ty, c->tz);
                                        glVertex3f (prev->tx, prev->ty, prev->tz);
                                    glEnd ();
                                    drawCam(c->tx,c->ty,c->tz,cam_size/6, c->rx, c->ry, c->rz);
                                    prev = c;
								}
							c = Item::ptr_cast_to<Camera> (item);
							drawCam (c->tx, c->ty, c->tz, cam_size, c->rx, c->ry, c->rz);
							break;
						}

						case Item::POINT3D:
							drawPoint3D (Item::ptr_cast_to<Point3d> (item), point_size);
							break;

						case Item::PCLOUD:
							drawPointCloud (Item::ptr_cast_to<PointCloud> (item));
							break;

						case Item::BITMAP:
							drawBitmap (Item::ptr_cast_to<Bitmap> (item));
							break;

						default: break;
						}
				}
		}
}

void Window3D::drawAxis ()
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

	drawCam (1, 0, 0, 0.02, 0, -90, 0);
	drawCam (0, 1, 0, 0.02, 90, 0, 0);
	drawCam (0, 0, 1, 0.02, 180, 0, 0);
}


// рисуем точки из полученного Sequence
void Window3D::drawPoint3D (Point3d::ptr p, GLfloat size)
{
	glPointSize (size);
	glTranslatef (0, 0, 0);

	glBegin (GL_POINTS);
	glColor3f (p->r, p->g, p->b);
	glVertex3d (p->x, p->y, p->z);
	glColor3f (1, 1, 1);            // ?
	glEnd ();
}

void Window3D::drawPointCloud (PointCloud::ptr pc)
{
	for (auto p : pc->cloud)
		drawPoint3D (p, 1);
}


void Window3D::drawBitmap (Bitmap::ptr bitmap)
{
	GLuint texture;
	unsigned char *data = bitmap->bitmap.data;

	glGenTextures (1, &texture);            // массив текстур
	glBindTexture (GL_TEXTURE_2D, texture); // привязываем к текстуре фотографии
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D (GL_TEXTURE_2D,
	              0,                 // уровень детализации исходное изображение
	              3,                 // RGB
	              bitmap->bitmap.cols, // ширина
	              bitmap->bitmap.rows, // высота
	              0,                 // ширина границы
	              GL_BGR,            // формат хранения пикселей в массиве
	              GL_UNSIGNED_BYTE,  // тип хранения пикселей в массиве
	              data                // указатель на массив данных
	              );

	glEnable (GL_TEXTURE_2D);       // разрешаем наложение текстур

	// выделение пространства для текстуры
	glBegin (GL_QUADS);
	glColor3f (1, 1, 1);
	glNormal3f (0.0, 0.0, 1.0);
	double v = (double) bitmap->bitmap.cols / bitmap->bitmap.rows;
	glTexCoord2d (1, 1); glVertex3f (0.0, 0.0, 0.0);
	glTexCoord2d (1, 0); glVertex3f (0.0, 1.0, 0.0);
	glTexCoord2d (0, 0); glVertex3f (v, 1.0, 0.0);
	glTexCoord2d (0, 1); glVertex3f (v, 0.0, 0.0);
	glEnd ();

	glDisable (GL_TEXTURE_2D);      // выключить текстурирование
	glDeleteTextures (1, &texture); // удалить текстуру
}

// рисуем Cam из полученного Sequence
void Window3D::drawCam (double x, double y, double z, double a, double rx, double ry, double rz)
{
	glPushMatrix ();                         // сохранить текущюю систему координат

	// повернуть и переместить систему координат
	glTranslatef (x, y, z);
	glRotatef (rx, 1.0f, 0.0f, 0.0f);
	glRotatef (ry, 0.0f, 1.0f, 0.0f);
	glRotatef (rz, 0.0f, 0.0f, 1.0f);

	// основание пирамиды
	glBegin (GL_QUADS);
	glColor3f (1.0f, 1.0f, 0.0f);        // Желтый
	glVertex3d (-a, -a, a);              // Верх лево
	glVertex3d (a, -a, a);               // Верх право квадрата (Зад)
	glVertex3d (+a, a, a);               // Низ право
	glVertex3d (-a, a, a);               // Низ лево
	glEnd ();

	glBegin (GL_TRIANGLES);
        glColor3f (1.0f, 1.0f, 1.0f);       // Белый
        glVertex3d (0, 0, -2 * a);          // Низ лево
        glVertex3d (a, -a, +a);             // Верх право квадрата (Низ)
        glVertex3d (-a, -a, +a);            // Верх лево

        glColor3f (1.0f, 0.0f, 0.0f);       // Красный
        glVertex3d (0, 0, -2 * a);          // Низ лево
        glVertex3d (a, a, +a);              // Верх право квадрата (Низ)
        glVertex3d (a, -a, +a);             // Верх лево

        glColor3f (0.0f, 0.0f, 1.0f);       // Синий
        glVertex3d (0, 0, -2 * a);          // Низ лево
        glVertex3d (-a, -a, +a);            // Верх право квадрата (Низ)
        glVertex3d (-a, a, +a);             // Верх лево

        glColor3f (0.0f, 1.0f, 0.0f);       // Зеленый
        glVertex3d (0, 0, -2 * a);          // Низ лево
        glVertex3d (-a, a, +a);             // Верх право квадрата (Низ)
        glVertex3d (a, a, +a);              // Верх лево
	glEnd ();

	glPopMatrix ();                          // Восстановление исходную систему координат
}
