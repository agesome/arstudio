#ifndef CONFIG_H
#define CONFIG_H

#include <memory>
#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

using namespace boost::property_tree;

/**
        This class handles import from XML files and storage of algorithm-specific
        settings in a tree structure.
 */

class Config
{
public:
	typedef std::shared_ptr<Config> ptr;
	typedef std::function <void (const std::string &, const std::string &) > importCallback_t;
	static ptr make (void);

	Config ()
	{
	};
	~Config ()
	{
	};

	/**
	        Return the value of a property, cast to type T.

	        \param prop dot-sepatared path to the setting, like "path.to.setting"
	 */

	template <typename T> T get (const std::string & prop)
	{
		return main_tree.get<T> ("root." + prop);
	}

	/**
	        Change or place a setting the tree.

	        \param prop path to the setting
	        \param value desired value
	 */

	void put (const std::string & prop, const std::string & value)
	{
		main_tree.put ("root." + prop, value);
		std::cout << "root." + prop << " " << value << std::endl;
	}

	void importXml (const std::string &);
	void setImportCallback (importCallback_t);
private:
	importCallback_t importCallback; // < Called for each value imported
	ptree main_tree; // < the main setting tree, contains settings for all algorithms
};
#endif // CONFIG_H
