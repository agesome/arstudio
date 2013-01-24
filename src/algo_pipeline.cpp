#include <algo_pipeline.hpp>

std::list <AlgoPipeline::module_t> AlgoPipeline::modules;

AlgoPipeline::AlgoPipeline ()
{
	int rc;

	try
		{
			rc = lt_dlinit ();
			if (rc != 0)
				throw rc;
		}
	catch (int e)
		{
			throw;
		}
}

AlgoPipeline::~AlgoPipeline ()
{
	lt_dlexit ();
}

void
AlgoPipeline::loadModules (void)
{
	filesystem::directory_iterator dir (MODULE_SEARCH_PATH);
	filesystem::directory_iterator end;
	module_t module;

// TODO: error checking
	for (; dir != end; dir++)
		{
			filesystem::path p = dir->path ();
			if (p.extension () == ".xml")
				if (Config::getInstance ().importXml (p))
					{
						module.name = p.stem ().string ();
						std::cout << "module data loaded: " << module.name << std::endl;
						module.handle = lt_dlopenext ((MODULE_SEARCH_PATH + module.name).c_str ());
						*(void * *) (&module.process) = lt_dlsym (module.handle, "process");
						modules.push_back (module);
					}
		}
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