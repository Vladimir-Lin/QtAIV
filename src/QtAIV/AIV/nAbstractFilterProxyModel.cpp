#include "nAivPrivate.hpp"

#define d ((AbstractFilterProxyModelPrivate *)D)

N::AbstractFilterProxyModel:: AbstractFilterProxyModel (QObject * parent)
                            : QSortFilterProxyModel    (          parent)
                            , D((void *)new AbstractFilterProxyModelPrivate(this))
{
  d->filterModel = new FilterModel(this) ;
}

N::AbstractFilterProxyModel::~AbstractFilterProxyModel(void)
{
  delete d ;
  D = NULL ;
}

N::AbstractFilterModel * N::AbstractFilterProxyModel::filterModel(void) const
{
  return d->filterModel ;
}

QVariant N::AbstractFilterProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Vertical) {
    if (role == Qt::SizeHintRole) {
      QSize s = QSortFilterProxyModel::headerData(section, orientation, role).toSize();
      s.setHeight(50);
      return s;
    }
  }
  return QSortFilterProxyModel::headerData(section, orientation, role) ;
}

void N::AbstractFilterProxyModel::setFilterModel(AbstractFilterModel * filterModel)
{
  if (d->filterModel){
    disconnect(d->filterModel);
  }
  d->filterModel = filterModel;
  connect(d->filterModel, SIGNAL(dataChanged(QModelIndex, QModelIndex)), this, SLOT(updateResult()));
  connect(d->filterModel, SIGNAL(modelReset()), this, SLOT(updateResult()));
  connect(d->filterModel, SIGNAL(modeChanged(AdvancedItemViews::FilterProxyMode)), this, SLOT(updateResult()));
  connect(d->filterModel, SIGNAL(matchModeChanged(AdvancedItemViews::FilterMatchMode)), this, SLOT(updateResult()));
  d->filterModel->setSourceModel(sourceModel());
}

void N::AbstractFilterProxyModel::setSourceModel(QAbstractItemModel * sourceModel)
{
  if (d->filterModel->sourceModel()){
    disconnect(d->filterModel->sourceModel(), 0, this, 0);
  }
  QSortFilterProxyModel::setSourceModel(sourceModel);
  d->filterModel->setSourceModel(sourceModel);
  connect(d->filterModel->sourceModel(), SIGNAL(modelReset()), this, SLOT(updateResult()));
  emitResultCountChanged();
}

void N::AbstractFilterProxyModel::updateResult(void)
{
  invalidate             ( ) ;
  emitResultCountChanged ( ) ;
}

void N::AbstractFilterProxyModel::emitResultCountChanged(void)
{
  if (rowCount() != d->lastResultCount){
    d->lastResultCount = rowCount();
    emit resultCountChanged(d->lastResultCount, d->filterModel->sourceModel()->rowCount());
  }
}
