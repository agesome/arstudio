#ifndef VIDEOSOURCEOPENCV_HPP
#define VIDEOSOURCEOPENCV_HPP

#ifdef HAVE_LIBMAGIC
#include <magic.h>
#endif

#include <QString>
#include <QSharedPointer>
#include <QtDebug>

#include <IVideoSource.hpp>

namespace arstudio {
class VideoSourceOpenCV : public IVideoSource
{
public:
  VideoSourceOpenCV (const QString &);
  bool init ();
  int frame_count ();
  bool go_to_frame (int);
  bool next_frame ();
  const cv::Mat image ();
  const cv::Mat depth_map ();
private:
  QSharedPointer<cv::VideoCapture> m_video_capture;
  QString                          m_source_file;
  int                              current_frame;
  int                              m_frame_count;
  cv::Mat                          m_current_image;
};
}
#endif
