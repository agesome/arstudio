#ifndef REPOSITORY_VIEW_H
#define REPOSITORY_VIEW_H

#include <QTreeWidget>
#include <QTreeWidgetItem>

#include <string>

#include <Repository.hpp>
#include <Scenegraph.hpp>

namespace Workspace
{
/**
 *      This widget displays the contents of repository by installing
 * necessary callbacks in it.
 *
 *      It also informs Scenegraph when user selects or deselects a
 * sequence to be shown.
 */
class RepositoryView : public QTreeWidget
{
  Q_OBJECT
public:
  RepositoryView (Repository::ptr,
                  Scenegraph::ptr,
                  QWidget * MainWindow = nullptr);

private:
  void set_callbacks (void);

  Scenegraph::ptr scenegraph_ptr;
  Repository::ptr repository_ptr;

signals:
  void selection_changed (void);

private slots:
  void on_item_changed (QTreeWidgetItem *, int);
};
}
#endif // REPOSITORY_VIEW_H
