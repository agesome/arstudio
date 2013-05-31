#include <QApplication>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QQuickWindow>

#include <IWManager.hpp>
#include <Logger.hpp>
#include <ScenegraphAggregator.hpp>
#include <Sequence.hpp>
#include <pipeline.hpp>

namespace as = arstudio;

int
main (int argc, char * argv[])
{
  QApplication  application (argc, argv);
  QQmlEngine    qml_engine;
  QQmlComponent qml_core (&qml_engine);

  QQmlContext                 * root_context = qml_engine.rootContext ();
  as::Repository::ptr           repository   = as::Repository::make ();
  as::ScenegraphAggregator::ptr aggregator   =
    as::ScenegraphAggregator::make ();

  QSharedPointer<Pipeline> p = QSharedPointer<Pipeline> (new Pipeline ());

  QApplication::setApplicationName ("arstudio");
  QApplication::setOrganizationName ("CVTeam");
  as::Logger::set_repository (repository);

  qmlRegisterType<as::Sequence> ("arstudio", 1, 0, "Sequence");
  qmlRegisterType<as::Scenegraph> ("arstudio", 1, 0, "Scenegraph");
  qmlRegisterType<as::ScenegraphAggregator> ("arstudio", 1, 0,
                                             "ScenegraphAggregator");
  qmlRegisterType<as::Repository> ("arstudio", 1, 0, "Repository");
  qmlRegisterType<as::IWManager> ("arstudio", 1, 0, "IWManager");

  qRegisterMetaType<as::RepositoryNode> ("RepositoryNode");

  qml_engine.setBaseUrl (QUrl::fromLocalFile ("@QML_ROOT@"));
  qml_engine.addImportPath ("@QML_ROOT@");
  qml_core.loadUrl (QUrl::fromLocalFile ("Core/Core.qml"));

  root_context->setContextProperty ("g_Pipeline", p.data ());
  root_context->setContextProperty ("g_Repository", repository.data ());
  root_context->setContextProperty ("g_SAggregator", aggregator.data ());

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
