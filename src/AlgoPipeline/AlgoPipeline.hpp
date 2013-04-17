#ifndef ALGO_PIPELINE_H
#define ALGO_PIPELINE_H

#include <list>
#include <memory>

#include <opencv2/highgui/highgui.hpp>

#include <Logger.hpp>
#include <Config.hpp>
#include <AlgoInterface.hpp>

@ALGO_INCLUDES@

/**
 *      This class handles image processing algorithms.
 */

class AlgoPipeline
{
public:
	typedef std::shared_ptr<AlgoPipeline> ptr; // < a convenience typedef

	AlgoPipeline (Config::ptr);
	~AlgoPipeline ();

	static ptr make (Config::ptr);
	void process_frame (const cv::Mat &, const cv::Mat &);
	void create_all (void);

private:
	void create_algorithm (IAbstractAlgorithm *);

	std::list <IAbstractAlgorithm *> algo_list; // < list of all created
	                                            // algorithms
};

#endif // ALGO_PIPELINE_H
