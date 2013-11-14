#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <QObject>
#include <QQmlListProperty>
#include <QList>
#include <QSet>
#include <QMutex>

#include <Sequence.hpp>

namespace arstudio {
/**
 * @brief Keeps track of all sequences to be displayed in a single ItemWindow
 *
 * This class is instantinied by ItemWindow QML component and is responsible for
 * tracking all sequences associated with that window. It also provides to
 * ScenegraphAggregator a QSet of all frames for which associated sequences have
 * items
 *
 * Since an ItemWindow cannot display a bitmaps and 3D items simultaneously, a
 * "locking" mechanism is in place, providing a hint whether one should add
 * sequences of a certain type at the moment; For example, when Scenegraph
 * contains a single Bitmap sequence, it is considered "locked" to BITMAP,
 * because it's associated ItemWindow will only display one bitmap.
 */
class Scenegraph : public QObject
{
  Q_OBJECT
  Q_ENUMS (LockType)
  Q_PROPERTY (LockType locked_to READ locked_to
              NOTIFY locked_to_changed)
public:
  enum LockType { NORMAL, BITMAP, NOT_LOCKED };
  typedef QSharedPointer<Scenegraph> ptr;
  typedef QList<Sequence *> SequenceList;

  Scenegraph (QObject * parent = nullptr);
  static ptr
  make (QObject * parent = nullptr);

  Q_INVOKABLE void add_sequence (Sequence *);
  Q_INVOKABLE void remove_sequence (Sequence *);

  const SequenceList sequences ();
  const QSet<int> frames ();
  void clear ();

  LockType locked_to () const;
private:
  SequenceList m_sequences;
  QSet<int>    m_frames;
  QMutex       m_frameset_lock;
  LockType     m_locked_to;
signals:
  void sequences_changed ();
  void locked_to_changed ();
private slots:
  void rebuild_frames ();
};
}

#endif // SCENEGRAPH_H
