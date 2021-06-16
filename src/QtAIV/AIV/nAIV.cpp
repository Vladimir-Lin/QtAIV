#include "nAivPrivate.hpp"

N::AdvancedItemViews:: AdvancedItemViews(void)
{
}

N::AdvancedItemViews::~AdvancedItemViews(void)
{
}

QModelIndex N::qSourceIndex(const QModelIndex & index)
{
  QModelIndex i(index);
  QAbstractProxyModel * proxy ;
  while((proxy = qobject_cast<QAbstractProxyModel*>((QAbstractProxyModel*)i.model())))
  {
    i = proxy->mapToSource(i) ;
  }
  return i;
}

const QAbstractItemModel * N::qSourceModel(const QModelIndex & index)
{
  QModelIndex i(index);
  QAbstractProxyModel* p;
  while ((p = qobject_cast<QAbstractProxyModel*>((QAbstractProxyModel*)i.model())))
  {
    i = p->mapToSource(i) ;
  }
  return i.model();
}

QAbstractItemModel * N::qSourceModel(QAbstractItemModel * model)
{
  QAbstractItemModel  * m = model                     ;
  QAbstractProxyModel * p                             ;
  while ((p = qobject_cast<QAbstractProxyModel*>(m))) {
    m = p->sourceModel()                              ;
  }                                                   ;
  return m                                            ;
}
