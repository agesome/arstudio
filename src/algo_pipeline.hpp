#ifndef ALGO_PIPELINE_H
#define ALGO_PIPELINE_H

#include <QDomDocument>
#include <QFile>
#include <opencv2/highgui/highgui.hpp>
#include <boost/filesystem.hpp>
#include <ltdl.h>
#include <string>
#include <iostream>
#include <list>
#include <logger.hpp>
#include <config.hpp>

using namespace boost;

class AlgoPipeline : public QObject
{
	Q_OBJECT
private:
	const std::string MODULE_SEARCH_PATH = "./modules/";

	typedef struct {
		bool (*process)(cv::Mat &, cv::Mat &);
		lt_dlhandle handle;
		std::string name;
	} module_t;

	static std::list <module_t> modules;
public:

	AlgoPipeline ();
	~AlgoPipeline ();

	void loadModules (Config *);
	void processFrame (cv::Mat &, cv::Mat &);
	bool importXml (const filesystem::path &, Config *);
	double * accessModuleVariable (std::string, std::string);
public slots:
	void itemChangedHandler (QStandardItem *);
};
#endif // ALGO_PIPELINE_H
