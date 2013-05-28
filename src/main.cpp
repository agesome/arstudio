#include <QApplication>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QQuickWindow>

#include <Logger.hpp>
#include <Scenegraph.hpp>
#include <Sequence.hpp>
#include <pipeline.hpp>

namespace as = arstudio;

int
main (int argc, char * argv[])
{
  QApplication  application (argc, argv);
  QQmlEngine    qml_engine;
  QQmlComponent qml_core (&qml_engine);

  QQmlContext       * root_context = qml_engine.rootContext ();
  as::Repository::ptr repository   = as::Repository::make ();

  QSharedPointer<Pipeline> p = QSharedPointer<Pipeline> (new Pipeline ());

  QApplication::setApplicationName ("arstudio");
  QApplication::setOrganizationName ("CVTeam");
  as::Logger::set_repository (repository);

  qmlRegisterType<arstudio::Sequence> ("arstudio", 1, 0, "Sequence");
  qmlRegisterType<arstudio::Scenegraph> ("arstudio", 1, 0, "Scenegraph");
  qmlRegisterType<arstudio::Repository> ("arstudio", 1, 0, "Repository");
  qmlRegisterType<arstudio::Item> ("arstudio", 1, 0, "DataItem");
  qmlRegisterType<arstudio::Camera> ("arstudio", 1, 0, "Camera");
  qmlRegisterType<arstudio::Point3D> ("arstudio", 1, 0, "Point3D");
  qmlRegisterType<arstudio::PointCloud> ("arstudio", 1, 0, "PointCloud");

  qRegisterMetaType<arstudio::RepositoryNode> ("RepositoryNode");

  qml_engine.setBaseUrl (QUrl::fromLocalFile ("@QML_ROOT@"));
  qml_engine.addImportPath ("@QML_ROOT@");
  qml_core.loadUrl (QUrl::fromLocalFile ("Core/Core.qml"));

  root_context->setContextProperty ("g_Pipeline", p.data ());
  root_context->setContextProperty ("g_Repository", repository.data ());

  QObject * window_ = qml_core.create ();

  if (window_ != nullptr)
    {
      QQuickWindow * window = qobject_cast<QQuickWindow *> (window_);
      window->show ();
      QObject::connect (&qml_engine, &QQmlEngine::quit,
                        [&application]() { application.exit (EXIT_SUCCESS); });

      return application.exec ();
    }

  qWarning () << "Component creation failed with following errors:";
  for (QQmlError error : qml_core.errors ())
    qWarning () << error.toString ();
  return EXIT_FAILURE;
}
