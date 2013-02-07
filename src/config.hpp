#ifndef CONFIG_H
#define CONFIG_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

using namespace boost::property_tree;

class Config
{
public:
	Config (const std::string &);
	~Config ();

	template <typename T> T get (const std::string & prop)
	{
		return pt.get<T> (prop);
	}

	void reload (void);
private:
	std::string xml_path;
	ptree pt;
};
#endif // CONFIG_H
