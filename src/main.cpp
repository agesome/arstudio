#include <QApplication>
#include <Core.hpp>

int
main (int argc, char * argv[])
{
  QApplication app (argc, argv);

  QApplication::setApplicationName ("arstudio");
  QApplication::setOrganizationName ("CVTeam");

  arstudio::Core core;

  core.show ();

  return app.exec ();
}
