#ifndef VIDEOSOURCEKINVIDEO_HPP
#define VIDEOSOURCEKINVIDEO_HPP

#include <QSharedPointer>
#include <QString>

#include <IVideoSource.hpp>
#include <filecapture.hpp>

namespace arstudio {
class VideoSourceKinvideo : public IVideoSource
{
public:
  VideoSourceKinvideo (const QString &);
  bool init (void);
  int frame_count (void);
  bool go_to_frame (int);
  bool next_frame (void);
  const cv::Mat image (void);
  const cv::Mat depth_map (void);
private:
  QSharedPointer<FileCapture> m_video_capture;
  QString                     m_source_file;
  int                         m_current_frame;
  int                         m_frame_count;
  cv::Mat                     m_current_image;
  cv::Mat                     m_current_dmap;
};
}
#endif
