#include <config_view.hpp>

ConfigView::ConfigView (QWidget * MainWindow) : QTreeWidget (MainWindow)
{
	setColumnCount (2);
	QStringList sl;
	sl << "Setting" << "Value";
	setHeaderLabels (sl);
}

void ConfigView::populate (void)
{
	std::function<void(const Config::tree_t &, QTreeWidgetItem *)> walk =
	[&walk, this](const Config::tree_t & tree, QTreeWidgetItem * paren)
	{
		int ci = 0;
		QTreeWidgetItem * i;
		for (auto it : tree)
			{
				if (!it.second.empty ())
					{
						i = new QTreeWidgetItem (paren);
						i->setText (0, it.first.c_str ());
						i->setExpanded (true);
						walk (it.second, i);
					}
				else
					{
						i = new QTreeWidgetItem (paren);
						i->setText (0, it.first.c_str ());
						i->setText (1, QString ("%0").arg (it.second.data ()));
					}
			}
	};

	walk (Config::getInstance ().rawTree (), invisibleRootItem ());
	expandAll ();
	setItemsExpandable (false);
	resizeColumnToContents (0);
	resizeColumnToContents (1);
}