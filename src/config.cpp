#include <config.hpp>

void
Config::importXml (const std::string & filename)
{
	ptree tree;

	read_xml (filename, tree);

	std::function<void(const ptree &, std::string)> walk =
	  [&walk, this](const ptree & t, std::string prefix)
		{
			for (auto it : t)
				{
					if (!it.second.empty ())
						walk (it.second, prefix + it.first + ".");
					else
						main_tree.put (prefix + it.first, it.second.get_value<std::string> ());
				}
		};

	walk (tree, "root.");
}

Config::ptr
Config::make (void)
{
	return boost::make_shared<Config> ();
}
