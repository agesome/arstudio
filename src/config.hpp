#ifndef CONFIG_H
#define CONFIG_H

#include <boost/property_tree/ptree.hpp>
#include <string>

class Config
{
public:
	typedef boost::property_tree::basic_ptree<std::string, double> tree_t;
	static Config & getInstance (void);

	void addProperty (const std::string, const double);
	double getProperty (const std::string);
	const tree_t & rawTree (void);
private:
	Config ();
	Config (const Config &);
	void operator= (const Config &);

	tree_t pt;
};

#endif // CONFIG_H