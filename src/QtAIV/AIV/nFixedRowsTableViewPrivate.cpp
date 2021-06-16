#include "nAivPrivate.hpp"

N::FixedRowsTableViewPrivate:: FixedRowsTableViewPrivate(FixedRowsTableView * tv)
{
  decorationProxy = 0  ;
  filterProxy     = 0  ;
  v               = tv ;
}

N::FixedRowsTableViewPrivate::~FixedRowsTableViewPrivate(void)
{
}
