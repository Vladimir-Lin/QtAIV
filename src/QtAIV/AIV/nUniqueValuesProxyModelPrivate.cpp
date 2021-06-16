#include "nAivPrivate.hpp"

N::UniqueValuesProxyModelPrivate:: UniqueValuesProxyModelPrivate(UniqueValuesProxyModel * pm)
{
  emptyValues = true ;
  modelColumn = 0    ;
  m           = pm   ;
}

N::UniqueValuesProxyModelPrivate::~UniqueValuesProxyModelPrivate(void)
{
}
