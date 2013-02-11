#ifndef CONFIG_H
#define CONFIG_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <iostream>

using namespace boost::property_tree;

class Config
{
public:
	typedef boost::shared_ptr<Config> ptr;
	static ptr make (void);

	Config ()
	{
	};
	~Config ()
	{
	};

	template <typename T> T get (const std::string & prop)
	{
		return main_tree.get<T> ("root." + prop);
	}

	void importXml (const std::string &);
private:
	ptree main_tree;
};
#endif // CONFIG_H
