#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <QObject>
#include <QQmlListProperty>
#include <QList>
#include <QSet>

#include <Sequence.hpp>

namespace arstudio {
/**
 * Keeps track of all sequences to be displayed
 *
 * This class is instantinied by ItemWindow QML component and is
 * responsible
 * of tracking all sequences associated with that window. It also provides
 * to
 * ScenegraphAggregator a QSet of all frames for which associated sequences
 * have items.
 */
class Scenegraph : public QObject
{
  Q_OBJECT
  Q_ENUMS (LockType)
  // FIXME: segfault when setting type to LockType
  Q_PROPERTY (int locked_to READ locked_to
              NOTIFY locked_to_changed)
public:
  enum LockType { NORMAL, BITMAP, NOT_LOCKED };
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

  const SequenceList sequences ();
  const QSet<int> frames ();
  void clear ();

  int locked_to () const;
private:
  SequenceList m_sequences;
  QSet<int>    m_frames;
  LockType     m_locked_to;
signals:
  void sequences_changed ();
  void locked_to_changed ();
private slots:
  void rebuild_frames ();
};
}

#endif // SCENEGRAPH_H
