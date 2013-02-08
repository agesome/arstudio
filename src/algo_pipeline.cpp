#include <algo_pipeline.hpp>

AlgoPipeline::AlgoPipeline ()
{
	Config * c = new Config ("../algorithms/adjuster.xml");
	IAbstractAlgorithm * adjuster = new ImageAdjusterAlgorithm (c);

	adjuster->create ();

	algorithms.push_back (adjuster);
}

void
AlgoPipeline::processFrame (cv::Mat & image, cv::Mat & dmap)
{
	for (auto it : algorithms)
		it->run (image, dmap);
	Logger::getInstance ().advanceFrame ();
}
