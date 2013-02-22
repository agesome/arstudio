#ifndef ALGO_PIPELINE_H
#define ALGO_PIPELINE_H

#include <opencv2/highgui/highgui.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <logger.hpp>
#include <config.hpp>
#include <algo_interface.hpp>

#include <exception>
#include <string>
#include <list>
#include <iostream>

#include <NewOfNewSlam/debugger_interface.hpp>

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
private:
	void create (IAbstractAlgorithm *);
};
#endif // ALGO_PIPELINE_H
