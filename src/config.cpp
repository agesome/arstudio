#include <config.hpp>

Config::Config (const std::string & filename)
{
	xml_path = filename;
	read_xml (filename, pt);
}

Config::~Config (void)
{
}

Config::ptr
Config::make (const std::string & str)
{
	return boost::make_shared<Config> (str);
}
