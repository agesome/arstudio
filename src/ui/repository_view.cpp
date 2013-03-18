#include <ui/repository_view.hpp>

namespace Workspace
{
RepositoryView::RepositoryView (Repository::ptr r, Scenegraph::ptr sp, QWidget * MainWindow) : QTreeWidget (MainWindow)
{
	scgr = sp;
	repo = r;
	setColumnCount (1);
	setHeaderLabel ("Repository");

	repo->on_branch_add = [this](std::string s)
												{
													QTreeWidgetItem * i = new QTreeWidgetItem (invisibleRootItem ());
													i->setText (0, s.c_str ());
													i->setExpanded (true);
												};
	repo->on_leaf_add = [this](std::string l, std::string br)
											{
												QTreeWidgetItem * paren = this->findItems (br.c_str (), Qt::MatchExactly).first ();
												QTreeWidgetItem * i = new QTreeWidgetItem (paren);
												i->setText (0, l.c_str ());
												i->setFlags (Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
												i->setCheckState (0, Qt::Checked);

												Repository::branch & b = repo->accessBranch (br);
												Sequence::ptr seq = (*(b.find (l))).second;
												scgr->addSequence (seq);
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
	std::string leaf = item->text (0).toStdString ();

	Repository::branch & b = repo->accessBranch (branch);
	Sequence::ptr seq = (*(b.find (leaf))).second;

	if (item->checkState (0) == Qt::Checked)
		{
			scgr->addSequence (seq);
		}
	else
		{
			scgr->removeSequence (seq);
		}
}
}
