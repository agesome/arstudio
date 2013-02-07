#ifndef ALGO_INTERFACE_HPP
#define ALGO_INTERFACE_HPP

#include <opencv2/highgui/highgui.hpp>
#include <config.hpp>

class IAbstractAlgorithm
{
public:
	virtual bool create () = 0;
	virtual bool run (cv::Mat &, cv::Mat &) = 0;
	virtual void reconfigure (void) = 0;

	IAbstractAlgorithm (Config * config)
	{
		this->config = config;
	};

	~IAbstractAlgorithm ()
	{
		delete config;
	}

protected:
	virtual void recreate (void)
	{
	};
	Config * config;
};
#endif // ALGO_INTERFACE_HPP
