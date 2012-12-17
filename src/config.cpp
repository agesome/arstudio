#include <config.hpp>

Config::Config ()
{
}

Config &
Config::getInstance (void)
{
	static Config instance;

	return instance;
}

void
Config::addProperty (const std::string path, const double value)
{
	pt.put (path, value);
}

double
Config::getProperty (const std::string path)
{
	return pt.get<double> (path);
}

const Config::tree_t &
Config::rawTree (void)
{
	return pt;
}