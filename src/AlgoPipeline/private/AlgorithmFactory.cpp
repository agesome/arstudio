#include <AlgorithmFactory.hpp>

AlgorithmFactory &
AlgorithmFactoryInstance ()
{
  static AlgorithmFactory instance;

  return instance;
}

void
AlgorithmFactory::register_algorithm_creator (CreateAlgorithmFunction f)
{
  m_algorithm_creator_list.push_back (f);
}

AlgorithmFactory::AlgorithmCreatorList
AlgorithmFactory::algorithm_creator_list()
{
  return m_algorithm_creator_list;
}
