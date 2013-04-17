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

  repo->newBranchCallback =
    [this](std::string s)
    {
      QTreeWidgetItem * i = new QTreeWidgetItem (invisibleRootItem ());
      i->setText (0, s.c_str ());
      i->setExpanded (true);
    };
  repo->branchRemovedCallback =
    [this](std::string s)
    {
      QTreeWidgetItem * br =
        this->findItems (s.c_str (), Qt::MatchExactly).first ();
      delete br;
    };
  repo->newSequenceCallback =
    [this](std::string l, std::string br)
    {
      QTreeWidgetItem * paren =
        this->findItems (br.c_str (), Qt::MatchExactly).first ();
      QTreeWidgetItem * i = new QTreeWidgetItem (paren);
      i->setText (0, l.c_str ());
      i->setFlags (
        Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
      i->setCheckState (0, Qt::Checked);

      Repository::sequenceMap & sm  = repo->getSequenceMap (br);
      Repository::mapItem       mi  = *(sm.find (l));
      Sequence::ptr             seq = mi.second;
      scgr->addSequence (seq);
    };
  repo->sequenceRemovedCallback =
    [this](std::string l, std::string br)
    {
      QTreeWidgetItem * i = this->findItems (br.c_str (), Qt::MatchExactly
                                             | Qt::MatchRecursive).first ();

      delete i;

      Repository::sequenceMap & sm  = repo->getSequenceMap (l);
      Repository::mapItem       mi  = *(sm.find (br));
      Sequence::ptr             seq = mi.second;
      scgr->removeSequence (seq);
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
  std::string branch = par->text (0).toStdString ();
  std::string leaf   = item->text (0).toStdString ();

  Repository::sequenceMap & sm  = repo->getSequenceMap (branch);
  Repository::mapItem       mi  = *(sm.find (leaf));
  Sequence::ptr             seq = mi.second;

  if (item->checkState (0) == Qt::Checked)
    scgr->addSequence (seq);
  else
    scgr->removeSequence (seq);
  selectionChanged ();
}
}
