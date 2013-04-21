#ifndef CONFIG_H
#define CONFIG_H

#include <memory>
#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace arstudio {
namespace pt = boost::property_tree;

/**
 * This class handles import from XML files and storage of
 * algorithm-specific settings in a tree structure.
 */

class Config
{
public:
  typedef std::shared_ptr <Config> ptr;
  typedef std::function <void (const std::string &,
                               const std::string &)> import_callback_t;

  void import_xml (const std::string &);
  void set_import_callback (import_callback_t);
  static ptr make (void);
  /**
   * Return the value of a property, cast to type T.
   *
   * \param prop dot-sepatared path to the setting, like
   * "path.to.setting"
   */
  template <typename T>
  inline T
  get (const std::string & prop)
  {
    return main_tree.get<T> ("root." + prop);
  }
  /**
   *      Change or place a setting in the tree.
   *
   *      \param prop path to the setting
   *      \param value desired value
   */
  inline void
  put (const std::string & prop, const std::string & value)
  {
    main_tree.put ("root." + prop, value);
  }
private:
  void walk_tree (const pt::ptree &, const std::string &);
  import_callback_t import_callback; ///< called for each value imported
  pt::ptree         main_tree; ///< contains settings for all algorithms
};
}

#endif // CONFIG_H
