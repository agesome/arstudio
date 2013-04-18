#include <RepositoryView.hpp>

namespace Workspace
{
RepositoryView::RepositoryView (Repository::ptr r,
                                Scenegraph::ptr sp,
                                QWidget * MainWindow) : QTreeWidget (MainWindow)
{
  scgr = sp;
  repo = r;
  setColumnCount (1);
  setHeaderLabel ("Repository");

  repo->new_branch_cb =
    [this](const std::string & branch)
    {
      QTreeWidgetItem * i = new QTreeWidgetItem (invisibleRootItem ());
      i->setText (0, branch.c_str ());
      i->setExpanded (true);
    };
  repo->removed_branch_cb =
    [this](const std::string & branch)
    {
      QTreeWidgetItem * br =
        this->findItems (branch.c_str (), Qt::MatchExactly).first ();
      delete br;
    };
  repo->new_sequence_cb =
    [this](const std::string & sequence, const std::string & branch)
    {
      QTreeWidgetItem * paren =
        this->findItems (branch.c_str (), Qt::MatchExactly).first ();
      QTreeWidgetItem * i = new QTreeWidgetItem (paren);
      i->setText (0, sequence.c_str ());
      i->setFlags (
        Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
      i->setCheckState (0, Qt::Checked);

      scgr->addSequence (repo->find_sequence (sequence, branch));
    };
  repo->removed_sequence_cb =
    [this](const std::string & sequence, const std::string & branch)
    {
      QTreeWidgetItem * i = this->findItems (sequence.c_str (), Qt::MatchExactly
                                             | Qt::MatchRecursive).first ();

      delete i;

      scgr->removeSequence (repo->find_sequence (sequence, branch));
    };

  connect (this, SIGNAL (itemClicked (QTreeWidgetItem *, int)), this,
           SLOT (onItemChanged (QTreeWidgetItem *, int)));
}

void
RepositoryView::onItemChanged (QTreeWidgetItem * item, int)
{
  QTreeWidgetItem * par = item->parent ();

  if (par == NULL)
    return;
  std::string sequence_map = par->text (0).toStdString ();
  std::string map_item     = item->text (0).toStdString ();

  Sequence::ptr seq = repo->find_sequence (map_item, sequence_map);

  if (item->checkState (0) == Qt::Checked)
    scgr->addSequence (seq);
  else
    scgr->removeSequence (seq);
  selectionChanged ();
}
}
