#ifndef ALGO_INTERFACE_HPP
#define ALGO_INTERFACE_HPP

#include <opencv2/highgui/highgui.hpp>
#include <config.hpp>

class IAbstractAlgorithm
{
public:

	virtual bool create () = 0;
	virtual bool run (cv::Mat &, cv::Mat &) = 0;

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
