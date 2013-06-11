#ifndef VIDEOHELPER_HPP
#define VIDEOHELPER_HPP

#include <QObject>
#include <QFileInfo>
#include <QSharedPointer>
#include <QUrl>

#include <opencv2/highgui/highgui.hpp>

#include <VideoSourceOpenCV.hpp>
#include <VideoSourceKinvideo.hpp>

namespace arstudio {
/**
 * Video loading helper
 *
 * This class provides a convenient, consistent interface for retrieving
 * frames
 * from video files of different types, using different sources
 */

class VideoHelper : public QObject
{
  Q_OBJECT
  Q_PROPERTY (QUrl source READ source_file WRITE set_source)
  Q_PROPERTY (bool source_status READ status)
  Q_PROPERTY (QString source_basename READ file_basename NOTIFY basename_changed)
  Q_PROPERTY (int frame_count READ frame_count NOTIFY frame_count_changed)
  Q_PROPERTY (bool status READ status NOTIFY status_changed)
public:
  VideoHelper (QObject * parent = nullptr);

  void set_source (const QUrl &);
  bool load_file (const QString &);
  const QString file_basename (void);
  const QUrl source_file (void);

  bool go_to_frame (int);
  bool next_frame (void);
  int frame_count (void);

  bool status (void);

  const cv::Mat image (void);
  const cv::Mat depth_map (void);
private:
  QSharedPointer<IVideoSource> m_video_source;
  QString                      m_file_basename;
  QUrl                         m_source_file;
  bool                         m_status;
  int                          m_frame_count;
signals:
  void basename_changed (void);
  void frame_count_changed (void);
  void status_changed (void);
};
}
#endif // VIDEOHELPER_HPP
