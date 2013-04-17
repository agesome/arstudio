#include <Config.hpp>

/**
 *      Import setting from an XML file
 *
 *      \param file_path path to the XML file
 */
void
Config::import_xml (const std::string & file_path)
{
	pt::ptree xml_tree;

	read_xml (file_path, xml_tree);
	walk_tree (xml_tree, "");
}

void
Config::walk_tree (const pt::ptree & tree, const std::string & prefix)
{
	for (auto node : tree)
		{
			if (!node.second.empty ())
				{
					walk_tree (node.second, prefix + node.first + ".");
				}
			else
				{
					std::string path  = prefix + node.first;
					std::string value = node.second.get_value<std::string> ();
					main_tree.put ("root." + path, value);
					if (import_callback)
						{
							import_callback (path, value);
						}
				}
		}
}

Config::ptr
Config::make (void)
{
	return std::make_shared<Config> ();
}

/**
 *      Set the import callback function
 *
 *      \param callback the function to be called
 */
void
Config::set_import_callback (import_callback_t callback)
{
	import_callback = callback;
}
