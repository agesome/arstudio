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
 * @brief This class is responsible for running all registered algotihms
 *
 * Data passed to process_frame is subsequently passed to an instance of every
 * registered algorithm
 */
class AlgoPipeline
{
public:
  typedef std::shared_ptr<AlgoPipeline> ptr;

  ~AlgoPipeline ();

  static ptr make ();

  /**
   * @brief Process a single frame; data is passed down to algorithms
   */
  void process_frame (const cv::Mat & image, const cv::Mat & dmap);

  /**
   * @brief Create instances of algorithms registered with AlgorithmFactory
   * @param config to be passed to algorithm constructors
   */
  void create_all (Config * config);
private:
  /**
   * @brief List of algorithm instances to be run
   */
  std::list <IAbstractAlgorithm *> m_algorithm_list;
};
}
#endif // ALGO_PIPELINE_H
