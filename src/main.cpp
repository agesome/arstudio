#include <QApplication>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QQuickWindow>
#include <QObject>

#include <IWManager.hpp>
#include <Logger.hpp>
#include <Repository.hpp>
#include <ScenegraphAggregator.hpp>
#include <Sequence.hpp>
#include <VideoHelper.hpp>
#include <VideoPipeline.hpp>

namespace as = arstudio;

int
main (int argc, char * argv[])
{
  QApplication  application (argc, argv);
  QQmlEngine    qml_engine;
  QQmlComponent qml_core (&qml_engine);

  QQmlContext       * root_context = qml_engine.rootContext ();
  as::Repository::ptr repository   = as::Repository::make ();

  QObject::connect (repository.data (),
                    &as::Repository::removing_all_nodes,
                    as::ScenegraphAggregator::instance (),
                    &as::ScenegraphAggregator::repository_clearing);

  QObject::connect (repository.data (),
                    &as::Repository::removing_all_nodes,
                    []() { as::Logger::instance ().reset_frame_counter (); });

  QApplication::setApplicationName ("arstudio");
  QApplication::setOrganizationName ("CVTeam");
  as::Logger::set_repository (repository);

  qmlRegisterType<as::Sequence> ("arstudio", 1, 0, "Sequence");
  qmlRegisterType<as::Scenegraph> ("arstudio", 1, 0, "Scenegraph");
  qmlRegisterType<as::Repository> ("arstudio", 1, 0, "Repository");
  qmlRegisterType<as::RepositoryNode> ("arstudio", 1, 0, "RepositoryNode");
  qmlRegisterType<as::IWManager> ("arstudio", 1, 0, "IWManager");
  qmlRegisterType<as::VideoHelper> ("arstudio", 1, 0, "VideoHelper");
  qmlRegisterType<as::VideoPipeline> ("arstudio", 1, 0, "VideoPipeline");

  qmlRegisterSingletonType<as::ScenegraphAggregator>
    ("arstudio", 1, 0, "SAggregator",
    &as::ScenegraphAggregator::qml_instance);

  qml_engine.setBaseUrl (QUrl::fromLocalFile ("@QML_ROOT@"));
  qml_engine.addImportPath ("@QML_ROOT@");
  qml_core.loadUrl (QUrl::fromLocalFile ("Core/Core.qml"));

  root_context->setContextProperty ("g_Repository", repository.data ());

  QObject * window_ = qml_core.create ();

  if (window_ != nullptr)
    {
      QQuickWindow * window = qobject_cast<QQuickWindow *> (window_);
      QObject::connect (&qml_engine, &QQmlEngine::quit,
                        [&application]() { application.exit (EXIT_SUCCESS); });
      window->show ();
      return application.exec ();
    }

  qWarning () << "Component creation failed with following errors:";
  for (QQmlError error : qml_core.errors ())
    qWarning () << error.toString ();
  return EXIT_FAILURE;
}
