#include "nAivPrivate.hpp"

N::AbstractFilterProxyModelPrivate:: AbstractFilterProxyModelPrivate(AbstractFilterProxyModel * pm)
{
  m               = pm ;
  filterModel     = 0  ;
  lastResultCount = -1 ;
}

N::AbstractFilterProxyModelPrivate::~AbstractFilterProxyModelPrivate(void)
{
}
