#include <algo_pipeline.hpp>

AlgoPipeline::AlgoPipeline (Config::ptr config)
{
	IAbstractAlgorithm * adjuster = new ImageAdjusterAlgorithm (config);

	create (adjuster);
}

void AlgoPipeline::create (IAbstractAlgorithm * a)
{
	if (a->create ())
		algorithms.push_back (a);
	else
		{
			// std::cout << "throw!" << std::endl;
			throw "Failed to create algo: " +
			      std::string (typeid(ImageAdjusterAlgorithm).name ());
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
