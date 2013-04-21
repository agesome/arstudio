#include <RepositoryView.hpp>

namespace arstudio {
RepositoryView::RepositoryView (Repository::ptr r,
                                Scenegraph::ptr sp,
                                QWidget * MainWindow) : QTreeWidget (MainWindow)
{
  scenegraph_ptr = sp;
  repository_ptr = r;
  setColumnCount (1);
  setHeaderLabel ("Repository");

  set_callbacks ();

  connect (this, SIGNAL (itemClicked (QTreeWidgetItem *, int)), this,
           SLOT (on_item_changed (QTreeWidgetItem *, int)));
}

void
RepositoryView::set_callbacks (void)
{
  repository_ptr->new_branch_cb =
    [this](const std::string & branch)
    {
      QTreeWidgetItem * i = new QTreeWidgetItem (invisibleRootItem ());
      i->setText (0, branch.c_str ());
      i->setExpanded (true);
    };

  repository_ptr->removed_branch_cb =
    [this](const std::string & branch)
    {
      QTreeWidgetItem * br =
        findItems (branch.c_str (), Qt::MatchExactly).first ();
      delete br;
    };

  repository_ptr->new_sequence_cb =
    [this](const std::string & sequence, const std::string & branch)
    {
      QTreeWidgetItem * paren =
        findItems (branch.c_str (), Qt::MatchExactly).first ();
      QTreeWidgetItem * i = new QTreeWidgetItem (paren);
      i->setText (0, sequence.c_str ());
      i->setFlags (Qt::ItemIsUserCheckable
                   | Qt::ItemIsSelectable
                   | Qt::ItemIsEnabled);
      i->setCheckState (0, Qt::Checked);

      scenegraph_ptr->add_sequence (repository_ptr->find_sequence (sequence,
                                                                   branch));
    };

  repository_ptr->removed_sequence_cb =
    [this](const std::string & sequence, const std::string & branch)
    {
      QTreeWidgetItem * i = findItems (sequence.c_str (), Qt::MatchExactly
                                       | Qt::MatchRecursive).first ();
      delete i;

      scenegraph_ptr->remove_sequence (repository_ptr->find_sequence (sequence,
                                                                      branch));
    };
}

void
RepositoryView::on_item_changed (QTreeWidgetItem * item, int)
{
  QTreeWidgetItem * par = item->parent ();

  if (par == NULL)
    return;
  std::string sequence_map = par->text (0).toStdString ();
  std::string map_item     = item->text (0).toStdString ();

  Sequence::ptr seq = repository_ptr->find_sequence (map_item, sequence_map);

  if (item->checkState (0) == Qt::Checked)
    scenegraph_ptr->add_sequence (seq);
  else
    scenegraph_ptr->remove_sequence (seq);
  selection_changed ();
}
}
