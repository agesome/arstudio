#ifndef CONFIG_H
#define CONFIG_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

using namespace boost::property_tree;

class Config
{
public:
	typedef boost::shared_ptr<Config> ptr;
	static ptr make (const std::string &);

	Config (const std::string &);
	~Config ();

	template <typename T> T get (const std::string & prop)
	{
		return pt.get<T> (prop);
	}
private:
	std::string xml_path;
	ptree pt;
};
#endif // CONFIG_H
