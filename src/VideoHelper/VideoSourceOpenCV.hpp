#ifndef VIDEOSOURCEOPENCV_HPP
#define VIDEOSOURCEOPENCV_HPP

#include <QString>
#include <QSharedPointer>

#include <IVideoSource.hpp>

namespace arstudio {
class VideoSourceOpenCV : public IVideoSource
{
public:
  VideoSourceOpenCV (const QString &);
  bool init (void);
  int frame_count (void);
  bool go_to_frame (int);
  bool next_frame (void);
  const cv::Mat image (void);
  const cv::Mat depth_map (void);
private:
  QSharedPointer<cv::VideoCapture> m_video_capture;
  QString                          m_source_file;
  int                              current_frame;
  int                              m_frame_count;
  cv::Mat                          m_current_image;
};
}
#endif
