#include <QApplication>
#include <Core.hpp>

int
main (int argc, char * argv[])
{
  QApplication::setApplicationName ("arstudio");
  QApplication::setOrganizationName ("CVTeam");

  qRegisterMetaType<std::string> ("std::string");
  qRegisterMetaType < QVector < int >> ("QVector<int>");

  QApplication   app (argc, argv);
  arstudio::Core core;

  core.show ();
  return app.exec ();
}
