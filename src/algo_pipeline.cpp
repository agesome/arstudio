#include <algo_pipeline.hpp>

AlgoPipeline::AlgoPipeline (Config::ptr config)
{
	config->importXml ("../algorithms/adjuster.xml");
	IAbstractAlgorithm * adjuster = new ImageAdjusterAlgorithm (config);
	algorithms.push_back (adjuster);
}

void
AlgoPipeline::create (void)
{
	for (auto it : algorithms)
		it->create ();
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
