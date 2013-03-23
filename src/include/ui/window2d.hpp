#ifndef WINDOW2D_H
#define WINDOW2D_H

#include <QImage>
#include <QLabel>

#include <stdexcept>

#include <scenegraph.hpp>
#include <bitmap.hpp>

#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace Workspace;

class Window2D : public QLabel
{
	Q_OBJECT
public:
	Window2D (Scenegraph::ptr, QWidget * parent = nullptr);
private:
	QPixmap currentPixmap;
	Scenegraph::ptr scenegraph;
	int currentFrame = 1;

	void resizeEvent (QResizeEvent *);

public slots:
	void update (int);
	void update (void);
};
#endif // WINDOW2D_H
