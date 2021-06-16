#include "nAivPrivate.hpp"

N::AbstractFilterModelPrivate:: AbstractFilterModelPrivate(AbstractFilterModel * fm)
{
  m = fm ;
}

N::AbstractFilterModelPrivate::~AbstractFilterModelPrivate(void)
{
  qDeleteAll ( filterGroupList ) ;
}
