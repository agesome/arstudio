#ifndef ALGORITHMFACTORY_HPP
#define ALGORITHMFACTORY_HPP

#include <Config.hpp>
#include <AlgoInterface.hpp>

namespace as = arstudio;

/**
 * @brief This class manages automatic registration of algorithms at run time
 */

class AlgorithmFactory {
public:
  typedef IAbstractAlgorithm * (* CreateAlgorithmFunction) (as::Config *);
  typedef std::list<CreateAlgorithmFunction> AlgorithmCreatorList;

  void
  register_algorithm_creator (CreateAlgorithmFunction f);

  AlgorithmCreatorList algorithm_creator_list ();

private:
  AlgorithmCreatorList m_algorithm_creator_list;
};

AlgorithmFactory & AlgorithmFactoryInstance ();

template <typename T>
/**
 * @brief Convenience class for algorithms registration
 *
 * Simply creating an instance of this class for an algorithm is sufficient
 * to register that algorithm with AlgorithmFactory
 */
class AlgorithmRegisterer
{
public:
  AlgorithmRegisterer ()
  {
    AlgorithmFactoryInstance ().register_algorithm_creator (creator);
  }

  static IAbstractAlgorithm *
  creator (as::Config * c)
  {
    return new T (c);
  }
};

#endif // ALGORITHMFACTORY_HPP
