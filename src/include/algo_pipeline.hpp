#ifndef ALGO_PIPELINE_H
#define ALGO_PIPELINE_H

#include <opencv2/highgui/highgui.hpp>
#include <boost/filesystem.hpp>
#include <string>
#include <iostream>
#include <list>
#include <logger.hpp>
#include <config.hpp>
#include <adjuster.hpp>
#include <algo_interface.hpp>

using namespace boost;

class AlgoPipeline
{
private:
public:
	std::list <IAbstractAlgorithm *> algorithms;

	AlgoPipeline ();
	~AlgoPipeline ();

	void processFrame (cv::Mat &, cv::Mat &);
};
#endif // ALGO_PIPELINE_H
