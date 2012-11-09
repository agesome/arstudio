#include <scenegraph_selector.hpp>

namespace Workspace
{

ScenegraphSelector::ScenegraphSelector (QMainWindow * MainWindow) : QWidget(MainWindow)
{
	layout = boost::make_shared<QVBoxLayout> (this);
}

void
ScenegraphSelector::addCheckbox (Item::type t)
{
	qbox_ptr box = boost::make_shared<QCheckBox>
	               (QString::fromStdString (Item::typeNames[t]));
	boxes.push_back (std::pair<Item::type, qbox_ptr>(t, box));
	layout->addWidget (box.get ());
}

Item::typemask
ScenegraphSelector::getSelections (void)
{
	Item::typemask m = 0;

	for (auto it: boxes)
		{
			if (it.second->isChecked ())
				m |= (1 << it.first);
		}
	return m;
}

}
