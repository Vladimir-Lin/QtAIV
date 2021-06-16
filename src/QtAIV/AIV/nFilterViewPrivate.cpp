#include "nAivPrivate.hpp"

N::FilterViewPrivate:: FilterViewPrivate(FilterView * fw)
{
  allowedFilterTypes  = 0xFFFF                      ;
  defaultFilterType   = AbstractFilter::Type        ;
  filterVisible       = true                        ;
  maxFilterVisible    = 3                           ;
  filterViewConnector = new FilterViewConnector(fw) ;
  v                   = fw                          ;
}

N::FilterViewPrivate::~FilterViewPrivate(void)
{
}
