#include <ConfigEditor.hpp>

/**
 *      The constructor
 *
 *      \param config the instance of Config to be edited
 *      \param parent parent widget
 */
ConfigEditor::ConfigEditor (Config::ptr config, QWidget * parent)
	: QTreeWidget (parent)
{
	this->config = config;
	Config::import_callback_t f = std::bind (&ConfigEditor::config_callback,
		this, std::placeholders::_1, std::placeholders::_2);
	config->set_import_callback (f);

	QStringList list;
	list << "Setting" << "Value";
	setHeaderLabels (list);

	connect (this, SIGNAL (itemChanged (QTreeWidgetItem *, int)), this,
		SLOT (item_changed_handler (QTreeWidgetItem *, int)));
}

/**
 *      This method gets set as the import callback for Config, allowing us
 * to see
 *      newly added settings
 *
 *      \param path path to the setting
 *      \param value the default value
 */
void
ConfigEditor::config_callback (const std::string & path,
                               const std::string & value)
{
	QStringList i;

	i << QString::fromStdString (path) << QString::fromStdString (value);
	QTreeWidgetItem * s = new QTreeWidgetItem (i);
	s->setFlags (s->flags () | Qt::ItemIsEditable);
	insertTopLevelItem (0, s);
	resizeColumnToContents (0);
	resizeColumnToContents (1);
}

/**
 *      This function is called when user edits a setting field. In turn,
 * it calls
 *      Config::put to update the setting.
 *
 *      \param item the field which was edited
 */
void
ConfigEditor::item_changed_handler (QTreeWidgetItem * item, int)
{
	config->put (item->text (0).toStdString (), item->text (1).toStdString ());
}
