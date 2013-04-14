#include <AlgoPipeline.hpp>

AlgoPipeline::AlgoPipeline (Config::ptr config)
{
	@ALGO_CODE@
}

void AlgoPipeline::create (IAbstractAlgorithm * a)
{
	try
		{
			if (a->create ())
				{
					algorithms.push_back (a);
				}
			else
				{
					throw std::runtime_error ("create() failed");
				}
		}
	catch (std::runtime_error & e)
		{
			throw e;
		}
}

AlgoPipeline::~AlgoPipeline ()
{
	for (auto it : algorithms)
		delete it;
}

AlgoPipeline::ptr
AlgoPipeline::make (Config::ptr config)
{
	return boost::make_shared<AlgoPipeline> (config);
}

void
AlgoPipeline::processFrame (cv::Mat & image, cv::Mat & dmap)
{
	for (auto it : algorithms)
		it->run (image, dmap);
	Logger::getInstance ().advanceFrame ();
}