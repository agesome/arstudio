#include <AlgoPipeline.hpp>

/**
        The constructor. During build, CMake inserts code specific to each algorithm
        being built.

        \param config is passed to every algorithm being instantified
 */

AlgoPipeline::AlgoPipeline (Config::ptr config)
{
	@ALGO_CODE@
}

/**
        Convenience function to initialize algorithms, or throw an exception if
        there's a problem.

        \param a pointer to an instance of a class adhering to IAbstractAlgorithm
 */

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

/**
        The destructor, in turn calls destructors for every algorithm.
 */

AlgoPipeline::~AlgoPipeline ()
{
	for (auto it : algorithms)
		delete it;
}

/**
        Convenience function. Creates an instance of AlgoPipeline wrapped in boost::shared_ptr.

        \param config see constructor
 */

AlgoPipeline::ptr
AlgoPipeline::make (Config::ptr config)
{
	return std::make_shared<AlgoPipeline> (config);
}

/**
        Processes a single frame of input with every algorithm.

        \param image RGB image to be passed to algorithms
        \param dmap depth map to be passed to algorithms
 */

void
AlgoPipeline::processFrame (cv::Mat & image, cv::Mat & dmap)
{
	for (auto it : algorithms)
		it->run (image, dmap);
	Logger::getInstance ().advanceFrame ();
}
