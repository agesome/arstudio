#include <ui/config_editor.hpp>

ConfigEditor::ConfigEditor (Config::ptr config, QWidget * parent)
	: QTreeWidget (parent)
{
	this->config = config;
	Config::importCallback_t f = std::bind (&ConfigEditor::configCallback, this, std::placeholders::_1, std::placeholders::_2);
	config->setImportCallback (f);

	QStringList list;
	list << "Setting" << "Value";
	setHeaderLabels (list);

	connect (this, SIGNAL (itemChanged (QTreeWidgetItem *, int)), this,
	         SLOT (itemChangedHandler (QTreeWidgetItem *, int)));
}

void
ConfigEditor::configCallback (std::string path, std::string name)
{
	QStringList i;

	i << QString::fromStdString (path) << QString::fromStdString (name);
	QTreeWidgetItem *s = new QTreeWidgetItem (i);
	s->setFlags (s->flags () | Qt::ItemIsEditable);
	insertTopLevelItem (0, s);
	resizeColumnToContents (0);
	resizeColumnToContents (1);
}

void
ConfigEditor::itemChangedHandler (QTreeWidgetItem * item, int column)
{
	std::cout << item->text (0).toStdString () << item->text (1).toStdString () << std::endl;

	config->put (item->text (0).toStdString (), item->text (1).toStdString ());
}
