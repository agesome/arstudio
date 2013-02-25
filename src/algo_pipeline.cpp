#include <algo_pipeline.hpp>

AlgoPipeline::AlgoPipeline (Config::ptr config)
{
	IAbstractAlgorithm * slam = new SlamInterface (config);

	create (slam);
}

void AlgoPipeline::create (IAbstractAlgorithm * a)
{
	if (a->create ())
		{
			algorithms.push_back (a);
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
