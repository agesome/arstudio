#include <config.hpp>

Config::Config ()
{
	setColumnCount (2);
	QStringList sl;
	sl << "Modules" << "";
	setHorizontalHeaderLabels (sl);
}
