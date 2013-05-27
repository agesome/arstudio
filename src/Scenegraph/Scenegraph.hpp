#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <QObject>
#include <QQmlListProperty>
#include <QList>

#include <climits>

#include <Sequence.hpp>

namespace arstudio {
/**
 *      Keeps track of all sequences to be displayed.
 */
class Scenegraph : public QObject
{
  Q_OBJECT
  Q_PROPERTY (QQmlListProperty<arstudio::Sequence> sequences READ sequences
              NOTIFY sequences_changed)
public:
  Q_INVOKABLE void add_sequence (Sequence *);
  Q_INVOKABLE void remove_sequence (Sequence *);
  Q_INVOKABLE unsigned int min_frame (void);
  Q_INVOKABLE unsigned int max_frame (void);

  QQmlListProperty<arstudio::Sequence> sequences (void);
private:
  QList<arstudio::Sequence *> sequences_;
signals:
  void sequences_changed (void);
};
}

#endif // SCENEGRAPH_H
