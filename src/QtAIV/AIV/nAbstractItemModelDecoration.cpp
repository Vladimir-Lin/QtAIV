#include "nAivPrivate.hpp"

#define d ((AbstractItemModelDecorationPrivate *)D)

N::AbstractItemModelDecoration:: AbstractItemModelDecoration (void)
                               : D((void *)new AbstractItemModelDecorationPrivate(this))
{
}

N::AbstractItemModelDecoration::~AbstractItemModelDecoration (void)
{
  delete d ;
  D = NULL ;
}

QIcon N::AbstractItemModelDecoration::defaultIcon(void) const
{
  return QIcon() ;
}

QVariant N::AbstractItemModelDecoration::property(const QString & key,const QVariant & defaultValue) const
{
  return d->properties.value(key,defaultValue) ;
}

WMAPs N::AbstractItemModelDecoration::properties(void) const
{
  return d->properties ;
}

bool N::AbstractItemModelDecoration::restoreState(const QByteArray & state)
{
  QDataStream s(state) ;
  QVariantMap m        ;
  s >> m               ;
  setProperties(m)     ;
  return true          ;
}

QByteArray N::AbstractItemModelDecoration::saveState(void) const
{
  QByteArray    ba   ;
  QDataStream s(ba)  ;
  s << d->properties ;
  return ba          ;
}

void N::AbstractItemModelDecoration::setProperty(const QString & name,const QVariant & value)
{
  d -> properties [ name ] = value ;
}

void N::AbstractItemModelDecoration::setProperties(const QVariantMap & properties)
{
  d -> properties = properties ;
}
