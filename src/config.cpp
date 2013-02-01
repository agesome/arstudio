#include <config.hpp>

Config::Config (const std::string & filename)
{
	read_xml (filename, pt);
}

Config::~Config (void)
{
}
