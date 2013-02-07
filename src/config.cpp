#include <config.hpp>

Config::Config (const std::string & filename)
{
	xml_path = filename;
	read_xml (filename, pt);
}

Config::~Config (void)
{
}

void
Config::reload (void)
{
	read_xml (xml_path, pt);
}
