#include <QApplication>
#include <ui/core.hpp>

using namespace Workspace;

int main (int argc, char * argv[])
{
	QApplication app (argc, argv);
	Core core;

	core.show ();

	return app.exec ();
}
