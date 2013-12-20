#ifndef VIDEOHELPER_HPP
#define VIDEOHELPER_HPP

#include <QObject>
#include <QFileInfo>
#include <QSharedPointer>
#include <QUrl>
#include <QSettings>

#include <opencv2/highgui/highgui.hpp>

#include <VideoSourceOpenCV.hpp>
#include <VideoSourceKinvideo.hpp>

namespace arstudio {
/**
 * @brief Video loading helper
 *
 * This class provides a convenient, consistent interface for retrieving frames
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
  ~VideoHelper ();

  /**
   * @brief Set the location of the video file to work with
   */
  void set_source (const QUrl & url);
  const QString file_basename ();
  const QUrl source_file ();

  bool go_to_frame (int frame);
  bool next_frame ();
  int frame_count ();

  /**
   * @brief File loading status
   * @return true on success, false otherwise
   */
  bool status ();

  const cv::Mat image ();
  const cv::Mat depth_map ();
private:
  QSharedPointer<IVideoSource> m_video_source;
  QString                      m_file_basename;
  QUrl                         m_source_file;
  QSettings                    settings;
  bool                         m_status; // is it still necessary?
  int                          m_frame_count;

  bool load_file (const QString &);
signals:
  void basename_changed ();
  void frame_count_changed ();
  void status_changed ();
};
}
#endif // VIDEOHELPER_HPP
