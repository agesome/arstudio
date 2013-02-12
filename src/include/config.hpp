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
	typedef std::function < void (std::string, std::string) > importCallback_t;
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

	void put (const std::string & prop, const std::string & value)
	{
		main_tree.put ("root." + prop, value);
		std::cout << "root." + prop << " " << value << std::endl;
	}

	void importXml (const std::string &);
	void setImportCallback (importCallback_t);
private:
	importCallback_t importCallback;
	bool haveImportCallback = false;
	ptree main_tree;
};
#endif // CONFIG_H
