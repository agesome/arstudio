#include <algo_pipeline.hpp>

std::list <AlgoPipeline::module_t> AlgoPipeline::modules;

AlgoPipeline::AlgoPipeline ()
{
	lt_dlinit ();
}

AlgoPipeline::~AlgoPipeline ()
{
	lt_dlexit ();
}

int
AlgoPipeline::module_load (const char * fn, void * data)
{
	module_t module;

	module.handle = lt_dlopenext (fn);
	if (module.handle == NULL)
		return 0;
	*(void * *) (&module.init) = lt_dlsym (module.handle, "_module_init");
	if (module.init == NULL)
		{
			lt_dlclose (module.handle);
			return 0;
		}
	*(void * *) (&module.process) = lt_dlsym (module.handle, "_module_process");
	*(void * *) (&module.reconfigure) = lt_dlsym (module.handle, "_module_reconfigure");
	module.desc = std::string ((char *) lt_dlsym (module.handle, "_module_desc"));
	module.name = std::string ((char *) lt_dlsym (module.handle, "_module_name"));
	if (module.init ())
		{
			std::cout << "Loaded module ";
			modules.push_back (module);
		}
	else
		std::cout << "Failed to load ";
	std::cout << fn << std::endl;

	return 0;
}

void
AlgoPipeline::loadModules (void)
{
	lt_dlforeachfile (MODULE_SEARCH_PATH, &AlgoPipeline::module_load, NULL);
}

void
AlgoPipeline::processFrame (Mat & image, Mat & dmap)
{
	for (auto m : modules)
		{
			Mat result;

			std::cout << "Processing with module: " << m.name << ": "
			          << (m.process (image, dmap) ? "success" : "failure") << std::endl;
		}
	Logger::getInstance ().advanceFrame ();
}