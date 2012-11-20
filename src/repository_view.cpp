#include <repository_view.hpp>

namespace Workspace
{
RepositoryView::RepositoryView (Repository::ptr r, Scenegraph::ptr sp, QWidget * MainWindow) : QTreeView (MainWindow)
{
	scgr = sp;
	repo = r;
	model = new QStandardItemModel ();
	setModel (model);
	model->setHorizontalHeaderItem (0, new QStandardItem ("Repository"));

	repo->on_branch_add = [this](std::string s)
	{
		QStandardItem * i = new QStandardItem (s.c_str ());

		i->setEditable (false);
		this->model->appendRow (i);
		this->child_count.insert (std::pair <std::string, int> (s, 0));
	};
	repo->on_leaf_add = [this](std::string l, std::string br)
	{
		QStandardItem * i = new QStandardItem (l.c_str ());
		QStandardItem * paren = this->model->findItems (br.c_str ()).first ();
		paren->setChild (child_count[br]++, 0, i);
		i->setEditable (false);
		i->setCheckable (true);
		i->setCheckState (Qt::Checked);
	};
	connect (model, SIGNAL (itemChanged (QStandardItem *)), this,
	         SLOT (onItemChanged (QStandardItem *)));
}

void
RepositoryView::onItemChanged (QStandardItem * item)
{
	QStandardItem * par = item->parent ();
	std::string branch = par->text ().toStdString ();
	std::string leaf = item->text ().toStdString ();

	Repository::branch & b = repo->accessBranch (branch);
	Sequence::ptr seq = (*(b.find (leaf))).second;

	if (item->checkState () == Qt::Checked)
		{
			scgr->addSequence (seq);
		}
	else
		{
			scgr->removeSequence (seq);
		}
}
}