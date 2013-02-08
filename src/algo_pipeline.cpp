#include <algo_pipeline.hpp>

AlgoPipeline::AlgoPipeline ()
{
	Config::ptr c = Config::make ("../algorithms/adjuster.xml");
	IAbstractAlgorithm * adjuster = new ImageAdjusterAlgorithm (c);

	adjuster->create ();

	algorithms.push_back (adjuster);
}

AlgoPipeline::~AlgoPipeline ()
{
	for (auto it : algorithms)
		delete it;
}

AlgoPipeline::ptr
AlgoPipeline::make (void)
{
	return boost::make_shared<AlgoPipeline> ();
}

void
AlgoPipeline::processFrame (cv::Mat & image, cv::Mat & dmap)
{
	for (auto it : algorithms)
		it->run (image, dmap);
	Logger::getInstance ().advanceFrame ();
}
