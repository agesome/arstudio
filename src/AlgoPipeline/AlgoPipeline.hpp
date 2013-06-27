#ifndef ALGO_PIPELINE_H
#define ALGO_PIPELINE_H

#include <list>
#include <memory>

#include <opencv2/highgui/highgui.hpp>

#include <Logger.hpp>
#include <Config.hpp>
#include <AlgoInterface.hpp>
#include <AlgorithmFactory.hpp>

namespace arstudio {
/**
 *      This class handles image processing algorithms.
 */

class AlgoPipeline
{
public:
  typedef std::shared_ptr<AlgoPipeline> ptr;

  ~AlgoPipeline ();

  static ptr make ();
  void process_frame (const cv::Mat &, const cv::Mat &);
  void create_all (Config *);
private:
  std::list <IAbstractAlgorithm *> m_algorithm_list;
};
}

#endif // ALGO_PIPELINE_H
