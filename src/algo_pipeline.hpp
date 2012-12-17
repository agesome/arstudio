#ifndef ALGO_PIPELINE_H
#define ALGO_PIPELINE_H

#include <opencv2/highgui/highgui.hpp>
#include <ltdl.h>
#include <string>
#include <iostream>
#include <list>
#include <logger.hpp>

using namespace cv;

class AlgoPipeline
{
private:
	const char * MODULE_SEARCH_PATH = "./plugins/";
	static int module_load (const char *, void *);

	typedef struct {
		bool (*init)(void);
		bool (*process)(Mat &, Mat &);
		void (*reconfigure)(void);
		lt_dlhandle handle;
		std::string desc;
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