#ifndef ALGO_PIPELINE_H
#define ALGO_PIPELINE_H

#include <opencv2/highgui/highgui.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <Logger.hpp>
#include <Config.hpp>
#include <AlgoInterface.hpp>

#include <exception>
#include <string>
#include <list>
#include <iostream>

@ALGO_INCLUDES@

class AlgoPipeline
{
public:
	typedef boost::shared_ptr<AlgoPipeline> ptr;

	AlgoPipeline (Config::ptr);
	~AlgoPipeline ();
	static ptr make (Config::ptr);

	void processFrame (cv::Mat &, cv::Mat &);
private:
	std::list <IAbstractAlgorithm *> algorithms;
	void create (IAbstractAlgorithm *);
};
#endif // ALGO_PIPELINE_H
