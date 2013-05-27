#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include <QObject>
#include <QtConcurrent>

#include <AlgoPipeline.hpp>
#include <Config.hpp>
#include <VideoHelper.hpp>

using namespace arstudio;

class Pipeline : public QObject
{
  Q_OBJECT
public:
  explicit
  Pipeline (QObject * parent = 0);
private:
  void process_frames (void);
public slots:
  void process (void);
signals:
  void progress (int v, int max);
};

#endif // PIPELINE_HPP
