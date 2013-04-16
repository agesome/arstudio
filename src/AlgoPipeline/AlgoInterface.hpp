#ifndef ALGO_INTERFACE_HPP
#define ALGO_INTERFACE_HPP

#include <string>

#include <opencv2/highgui/highgui.hpp>

#include <Config.hpp>

/**
        An interface for all processing algorithms.
 */

class IAbstractAlgorithm
{
public:
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
	virtual bool run (const cv::Mat & image, const cv::Mat & dmap) = 0;

	/**
	        Returns an identifier string for this algorithm.
	 */
	virtual const std::string id_string (void) = 0;

protected:
	Config::ptr config;
};
#endif // ALGO_INTERFACE_HPP
