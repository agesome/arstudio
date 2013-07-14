#ifndef REPOSITORY_NODE_HPP
#define REPOSITORY_NODE_HPP

#include <QString>
#include <QObject>
#include <Sequence.hpp>

namespace arstudio {
/**
 * @brief This class represents a single item in the Repository model
 */

class RepositoryNode : public QObject
{
  Q_OBJECT
  Q_PROPERTY (int type READ type CONSTANT)
  Q_PROPERTY (arstudio::Sequence * ptr READ ptr CONSTANT)
public:
  RepositoryNode (const QString & name,
                  Sequence::ptr data,
                  QObject * parent = nullptr)
    :
    QObject (parent),
    m_name (name),
    m_data (data)
  {
  }

  RepositoryNode () = default;

  arstudio::Sequence *
  ptr ()
  {
    return m_data.data ();
  }

  Sequence::ptr
  shared_ptr ()
  {
    return m_data;
  }

  const QString
  name ()
  {
    return m_name;
  }

  int
  type ()
  {
    return m_data->type ();
  }

private:
  Q_DISABLE_COPY (RepositoryNode)
  const QString m_name;
  Sequence::ptr m_data;
};
}
#endif
