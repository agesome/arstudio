#include <QApplication>
#include <Core.hpp>

using namespace Workspace;

int
main (int argc, char * argv[])
{
	QApplication app (argc, argv);

	QApplication::setApplicationName ("arstudio");
	QApplication::setOrganizationName ("LuxoftLab");

	Core core;

	core.show ();

	return app.exec ();
}
