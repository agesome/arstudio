#ifndef ALGO_INTERFACE_HPP
#define ALGO_INTERFACE_HPP

#include <opencv2/highgui/highgui.hpp>
#include <Config.hpp>

/**
        An interface for all processing algorithms.
 */

class IAbstractAlgorithm
{
public:
	/**
	        Must initialize all algorithm-specific structures and
	        return true in case of success, false otherwise. Must not throw.
	 */
	virtual bool create () = 0;
	/**
	        Passes frame data to underlying algorithm.

	        \param image RGB image
	        \param dmap depth map
	 */
	virtual bool run (cv::Mat & image, cv::Mat & dmap) = 0;

	/**
	        Stores a pointer to Config passed by AlgoPipeline() for this instance.

	        \param config pointer to an instance of Config
	 */
	IAbstractAlgorithm (Config::ptr config)
	{
		this->config = config;
	};
	virtual ~IAbstractAlgorithm(void)
	{
	};

protected:
	Config::ptr config;
};
#endif // ALGO_INTERFACE_HPP
