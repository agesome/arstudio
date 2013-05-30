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
public:
  typedef QSharedPointer<Scenegraph> ptr;
  typedef QList<Sequence *> SequenceList;

  Scenegraph (QObject * parent = nullptr);
  static ptr
  make (QObject * parent = nullptr)
  {
    return ptr (new Scenegraph (parent));
  }

  Q_INVOKABLE void add_sequence (Sequence *);
  Q_INVOKABLE void remove_sequence (Sequence *);
  int min_frame (void);
  int max_frame (void);

  const SequenceList sequences (void);
private:
  SequenceList m_sequences;
signals:
  void sequences_changed (void);
};
}

#endif // SCENEGRAPH_H
