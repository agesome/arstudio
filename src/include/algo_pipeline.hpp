#ifndef ALGO_PIPELINE_H
#define ALGO_PIPELINE_H

#include <opencv2/highgui/highgui.hpp>
#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
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
	typedef boost::shared_ptr<AlgoPipeline> ptr;

	std::list <IAbstractAlgorithm *> algorithms;

	AlgoPipeline (Config::ptr);
	~AlgoPipeline ();
	static ptr make (Config::ptr);

	void processFrame (cv::Mat &, cv::Mat &);
};
#endif // ALGO_PIPELINE_H
