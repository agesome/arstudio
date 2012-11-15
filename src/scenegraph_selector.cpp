#include <scenegraph_selector.hpp>

namespace Workspace
{
ScenegraphSelector::ScenegraphSelector (QWidget * MainWindow) : QWidget (MainWindow)
{
	layout = new QVBoxLayout (this);
}

void
ScenegraphSelector::addCheckbox (Item::type t)
{
	QCheckBox * box = new QCheckBox
											(QString::fromStdString (Item::typeNames[t]));

	box->setChecked (true);
	boxes.push_back (std::pair<Item::type, QCheckBox *>(t, box));
	layout->addWidget (box);
}

Item::typemask
ScenegraphSelector::getSelections (void)
{
	Item::typemask m = 0;

	for (auto it : boxes)
		{
			if (it.second->isChecked ())
				m |= (1 << it.first);
		}
	return m;
}
}
