#ifndef ALGO_PIPELINE_H
#define ALGO_PIPELINE_H

#include <exception>
#include <string>
#include <list>
#include <iostream>
#include <memory>

#include <opencv2/highgui/highgui.hpp>

#include <Logger.hpp>
#include <Config.hpp>
#include <AlgoInterface.hpp>

@ALGO_INCLUDES@

/**
        This class handles image processing algorithms.
 */

class AlgoPipeline
{
public:
	typedef std::shared_ptr<AlgoPipeline> ptr; // < a convenience typedef

	AlgoPipeline (Config::ptr);
	~AlgoPipeline ();
	static ptr make (Config::ptr);

	void processFrame (cv::Mat &, cv::Mat &);
private:
	std::list <IAbstractAlgorithm *> algorithms; // < list of all created algorithms
	void create (IAbstractAlgorithm *);
};
#endif // ALGO_PIPELINE_H
