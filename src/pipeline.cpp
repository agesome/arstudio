#include "pipeline.hpp"
#include <iostream>

Pipeline::Pipeline (QObject * parent) :
  QObject (parent)
{
}

void
Pipeline::process (void)
{
  QtConcurrent::run (this, &Pipeline::process_frames);
}

void
Pipeline::process_frames (void)
{
  std::cout << "processing method called (C++)" << std::endl;

  VideoHelper * video_helper = new VideoHelper ();
  Config::ptr   config       = Config::make ();

  config->import_xml (
    "/home/age/Sources/arstudio/src/algorithms/slam/settings.xml");
  AlgoPipeline::ptr algo_pipeline = AlgoPipeline::make (config);

  algo_pipeline->create_all ();
  video_helper->load_file ("/home/age/test.kinvideo");
  video_helper->go_to_frame (1);

  std::cout << "objects created (C++)" << std::endl;

  int i  = 1;
  int fc = video_helper->frame_count ();
  do
    {
      algo_pipeline->process_frame (video_helper->image (),
                                    video_helper->depth_map ());
      progress (i, fc);
      std::cout << "progress signal called: " << i << " (C++)" << std::endl;
      ++i;
    } while (video_helper->next_frame ());

  delete video_helper;
  std::cout << "processing method done (C++)" << std::endl;
}
