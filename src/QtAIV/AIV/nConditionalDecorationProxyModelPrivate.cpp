#include "nAivPrivate.hpp"

N::ConditionalDecorationProxyModelPrivate:: ConditionalDecorationProxyModelPrivate(ConditionalDecorationProxyModel * pm)
{
  iconSize    = QSize ( 16 , 16 ) ;
  iconSpacing = 3                 ;
  m           = pm                ;
}

N::ConditionalDecorationProxyModelPrivate::~ConditionalDecorationProxyModelPrivate(void)
{
  qDeleteAll ( columnDecorationMap ) ;
}
