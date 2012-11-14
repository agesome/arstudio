#include <QApplication>
#include <repository.hpp>
#include <scenegraph.hpp>
#include <window3d.hpp>
#include <point3d.hpp>
#include <camera.hpp>
#include <core.hpp>

using namespace Workspace;

int main (int argc, char * argv[])
{
	QApplication app (argc, argv);
    Core core;
    core.show();

    return app.exec ();
}
