#include <Config.hpp>

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
						{
							std::string path = prefix + it.first;
							std::string value = it.second.get_value<std::string> ();
							main_tree.put ("root." + path, value);
							if (importCallback)
								importCallback (path, value);
						}
				}
		};

	walk (tree, "");
}

Config::ptr
Config::make (void)
{
	return boost::make_shared<Config> ();
}

void
Config::setImportCallback (importCallback_t callback)
{
	importCallback = callback;
}
