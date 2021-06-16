#include "nAivPrivate.hpp"

#define d ((SingleColumnProxyModelPrivate *)D)

N::SingleColumnProxyModel:: SingleColumnProxyModel (QObject * parent)
                          : QSortFilterProxyModel  (          parent)
                          , D((void *)new SingleColumnProxyModelPrivate(this))
{
}

N::SingleColumnProxyModel::~SingleColumnProxyModel(void)
{
  delete d ;
  D = NULL ;
}

bool N::SingleColumnProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent);
    return source_column == d->modelColumn;
}

void N::SingleColumnProxyModel::setSourceModelColumn(int column)
{
    d->modelColumn = column;
    invalidate();
}

int N::SingleColumnProxyModel::sourceModelColumn() const
{
    return d->modelColumn;
}
