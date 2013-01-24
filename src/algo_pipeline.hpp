#ifndef ALGO_PIPELINE_H
#define ALGO_PIPELINE_H

#include <opencv2/highgui/highgui.hpp>
#include <boost/filesystem.hpp>
#include <ltdl.h>
#include <string>
#include <iostream>
#include <list>
#include <logger.hpp>
#include <config.hpp>

using namespace cv;
using namespace boost;

class AlgoPipeline
{
private:
	const std::string MODULE_SEARCH_PATH = "./modules/";

	typedef struct {
		bool (*process)(Mat &, Mat &);
		lt_dlhandle handle;
		std::string name;
	} module_t;

	static std::list <module_t> modules;
public:

	AlgoPipeline ();
	~AlgoPipeline ();

	void loadModules (void);
	void processFrame (Mat &, Mat &);
};

#endif // ALGO_PIPELINE_H