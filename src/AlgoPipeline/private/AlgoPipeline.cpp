#include <AlgoPipeline.hpp>

namespace arstudio {
void
AlgoPipeline::create_all (Config * config)
{
  for (auto f : AlgorithmFactoryInstance ().algorithm_creator_list ())
    m_algorithm_list.push_back (f (config));

  for (auto algo : m_algorithm_list)
    if (!algo->create ())
      throw std::runtime_error ("failed to create(): " + algo->id_string ());
}

AlgoPipeline::~AlgoPipeline ()
{
  for (auto algo : m_algorithm_list)
    delete algo;
}

AlgoPipeline::ptr
AlgoPipeline::make ()
{
  return std::make_shared<AlgoPipeline> ();
}

void
AlgoPipeline::process_frame (const cv::Mat & image, const cv::Mat & dmap)
{
  for (auto algo : m_algorithm_list)
    algo->run (image, dmap);
  Logger::instance ().advance_frame ();
}
}
