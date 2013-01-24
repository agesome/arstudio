#include <config.hpp>

Config::Config ()
{
	setColumnCount (2);
	QStringList sl;
	sl << "Modules" << "";
	setHorizontalHeaderLabels (sl);
}

Config &
Config::getInstance (void)
{
	static Config instance;

	return instance;
}

bool
Config::importXml (const filesystem::path & fp)
{
	static int row = 0;
	int subrow = 0;

	QString fpath = QString::fromStdString (fp.string ());
	QFile desc_file (fpath);
	QDomDocument desc;
	QStandardItem   *i, *c;

	if (!desc_file.open (QIODevice::ReadOnly))
		{
			return false;
		}
	if (!desc.setContent (&desc_file))
		{
			desc_file.close ();
			return false;
		}
	desc_file.close ();
	QDomElement root = desc.documentElement ();
	if (root.tagName () != "module")
		return false;
	QString moduleName = QString::fromStdString (fp.stem ().string ());
	i = new QStandardItem (moduleName);
	i->setEditable (false);
	insertRow (row++, i);

	QDomElement p = root.firstChildElement ("parameter");
	for (; p != QDomElement (); p = p.nextSiblingElement ("parameter"))
		{
			c = new QStandardItem (p.firstChildElement ("symbol").text ());
			i->setChild (subrow, 0, c);
			c->setEditable (false);
			c = new QStandardItem (p.firstChildElement ("value").text ());
			i->setChild (subrow++, 1, c);
		}

	return true;
}
