#include <scenegraph.hpp>

namespace Workspace
{

Scenegraph::Scenegraph (QMainWindow * MainWindow) : QWidget(MainWindow)
{
	layout = boost::make_shared<QVBoxLayout> (this);
}

void
Scenegraph::addCheckbox (int t)
{
	qbox_ptr box = boost::make_shared<QCheckBox>
								(QString::fromStdString (itemTypeNames[t]));
	boxes.push_back (std::pair< int, qbox_ptr >(t, box));
	layout->addWidget (box.get ());
}

void
Scenegraph::addSequence (Sequence::ptr seq)
{
	for (auto it: boxes)
		{
			if (it.first == seq->getType () && it.second->isChecked ())
				sequences.push_back (seq);
		}
}

}
