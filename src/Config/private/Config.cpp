#include <Config.hpp>

/**
        Import setting from an XML file

        \param file_path path to the XML file
 */

void
Config::importXml (const std::string & file_path)
{
	ptree tree;

	read_xml (file_path, tree);

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

/**
        Set the import callback function

        \param callback the function to be called
 */

void
Config::setImportCallback (importCallback_t callback)
{
	importCallback = callback;
}
