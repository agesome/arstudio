#include <config_view.hpp>

ConfigView::ConfigView (QWidget * MainWindow) : QTreeView (MainWindow)
{
	QHeaderView *v = new QHeaderView (Qt::Horizontal);

	v->setResizeMode (QHeaderView::ResizeToContents);
	v->setStretchLastSection (true);
	setHeader (v);
}

void
ConfigView::itemChangedHandler (QStandardItem *item)
{
	// TODO: update algorithm parameters in-memory
}
