// enable/disable QML debugging; better done here than via CMake
#if 0
#define QT_QML_DEBUG
#endif

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <ItemView.hpp>
#include <Logger.hpp>
#include <Repository.hpp>
#include <ScenegraphAggregator.hpp>
#include <Sequence.hpp>
#include <VideoHelper.hpp>
#include <VideoPipeline.hpp>
#include <Config.hpp>
#include <BitmapView.hpp>

namespace as = arstudio;

static void register_qml_types ();

int
main (int argc, char * argv[])
{
  QApplication          application (argc, argv);
  QQmlApplicationEngine engine;

  as::Repository::ptr repository = as::Repository::make ();
  as::Config::ptr     config     = as::Config::make ();

  QObject::connect (repository.data (),
                    &as::Repository::removing_all_nodes,
                    as::ScenegraphAggregator::instance (),
                    &as::ScenegraphAggregator::repository_clearing);

  QObject::connect (repository.data (),
                    &as::Repository::removing_all_nodes,
                    []() { as::Logger::instance ().reset_frame_counter (); });

  as::Logger::set_repository (repository);

  config->import_directory ("@CONFIG_DIRECTORY@");

  QApplication::setApplicationName ("arstudio");
  QApplication::setOrganizationName ("CVTeam");

  register_qml_types ();
  engine.setBaseUrl (QUrl ("qrc:///"));
  engine.addImportPath ("qrc:/");
  engine.addImportPath ("@QT3D_IMPORT_PATH@");
  engine.rootContext ()->setContextProperty ("g_Repository",
                                             repository.data ());
  engine.rootContext ()->setContextProperty ("g_Config", config.data ());
  engine.load (QUrl ("qrc:///Core/Core.qml"));
  return application.exec ();
}

static void
register_qml_types ()
{
  qmlRegisterType<as::Sequence> ("arstudio", 1, 0, "Sequence");
  qmlRegisterType<as::Scenegraph> ("arstudio", 1, 0, "Scenegraph");
  qmlRegisterType<as::Repository> ("arstudio", 1, 0, "Repository");
  qmlRegisterType<as::RepositoryNode> ("arstudio", 1, 0, "RepositoryNode");
  qmlRegisterType<as::ItemView> ("arstudio", 1, 0, "ItemView");
  qmlRegisterType<as::VideoHelper> ("arstudio", 1, 0, "VideoHelper");
  qmlRegisterType<as::VideoPipeline> ("arstudio", 1, 0, "VideoPipeline");
  qmlRegisterType<as::Config> ("arstudio", 1, 0, "Config");
  qmlRegisterType<as::BitmapView> ("arstudio", 1, 0, "BitmapView");

  qmlRegisterSingletonType<as::ScenegraphAggregator>
    ("arstudio", 1, 0, "SAggregator",
    &as::ScenegraphAggregator::qml_instance);
}
