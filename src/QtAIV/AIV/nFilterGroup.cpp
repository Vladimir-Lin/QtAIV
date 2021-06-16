#include "nAivPrivate.hpp"

N::FilterGroup:: FilterGroup(void)
{
}

N::FilterGroup:: FilterGroup(const QString & name)
{
  cName = name ;
}

N::FilterGroup::~FilterGroup(void)
{
  qDeleteAll ( cFilterList ) ;
}

void N::FilterGroup::add(AbstractFilter * filter)
{
  cFilterList . append ( filter ) ;
}

int N::FilterGroup::count(void) const
{
  return cFilterList . size ( ) ;
}

N::AbstractFilter * N::FilterGroup::filter(int index) const
{
  if (index < cFilterList.size()) {
    return cFilterList.at(index)  ;
  }                               ;
  return NULL                     ;
}

N::AbstractFilter * N::FilterGroup::filterAtColumn(int column) const
{
  Q_FOREACH(AbstractFilter * mFilter, cFilterList)  {
    if (mFilter->column() == column) return mFilter ;
  }                                                 ;
  return NULL                                       ;
}

QList<N::AbstractFilter *> N::FilterGroup::filters(void) const
{
  return cFilterList ;
}

bool N::FilterGroup::hasFilter(int index) const
{
  Q_FOREACH(AbstractFilter * mFilter,cFilterList) {
    if (mFilter->column() == index) return true   ;
  }                                               ;
  return false                                    ;
}

QString N::FilterGroup::name(void) const
{
  return cName ;
}

bool N::FilterGroup::remove(int column)
{
  for (int iFilter = 0; iFilter < cFilterList.size(); iFilter++) {
    if (cFilterList.at(iFilter)->column() == column)             {
      cFilterList.removeAt(iFilter)                              ;
      return true                                                ;
    }                                                            ;
  }                                                              ;
  return false                                                   ;
}

void N::FilterGroup::setName(const QString & name)
{
  cName = name ;
}
