#ifndef ALGORITHMFACTORY_HPP
#define ALGORITHMFACTORY_HPP

#include <Config.hpp>
#include <AlgoInterface.hpp>

namespace as = arstudio;

class AlgorithmFactory {
public:
  typedef IAbstractAlgorithm * (* CreateAlgorithmFunction) (as::Config *);
  typedef std::list<CreateAlgorithmFunction> AlgorithmCreatorList;

  void register_algorithm_creator(CreateAlgorithmFunction f);

  AlgorithmCreatorList algorithm_creator_list ();

private:
  AlgorithmCreatorList m_algorithm_creator_list;
};

AlgorithmFactory & AlgorithmFactoryInstance ();

template<typename T>
class AlgorithmRegisterer
{
public:
  AlgorithmRegisterer ()
  {
    AlgorithmFactoryInstance().register_algorithm_creator(creator);
  }

  static IAbstractAlgorithm * creator (as::Config * c)
  {
    return new T (c);
  }
};

#endif // ALGORITHMFACTORY_HPP
