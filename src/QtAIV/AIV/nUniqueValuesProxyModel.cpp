#include "nAivPrivate.hpp"

#define d ((UniqueValuesProxyModelPrivate *)D)

N::UniqueValuesProxyModel:: UniqueValuesProxyModel (QObject * parent)
                          : QSortFilterProxyModel  (          parent)
                          , D((void *)new UniqueValuesProxyModelPrivate(this))
{
}

N::UniqueValuesProxyModel::~UniqueValuesProxyModel(void)
{
  delete d ;
  D = NULL ;
}

QVariant N::UniqueValuesProxyModel::data(const QModelIndex & index, int role) const
{
    return mapToSource(index).data(role);
}

bool N::UniqueValuesProxyModel::emptyItemsAllowed() const
{
        return d->emptyValues;
}

bool N::UniqueValuesProxyModel::filterAcceptsRow(int source_row, const QModelIndex & source_parent) const
{
    return isDuplicate(source_row);
}

bool N::UniqueValuesProxyModel::insertRows(int row, int count, const QModelIndex & parent)
{
    return QSortFilterProxyModel::insertRows(row, count, parent);
}

int N::UniqueValuesProxyModel::modelColumn() const
{
    return d->modelColumn;
}

void N::UniqueValuesProxyModel::setModelColumn(int colum)
{
    beginResetModel();
    d->modelColumn = colum;
        buildMap();
    endResetModel();
}

void N::UniqueValuesProxyModel::setSourceModel(QAbstractItemModel* sourceModel)
{
    connect(sourceModel, SIGNAL(layoutChanged()), this, SLOT(buildMap()));
    connect(sourceModel, SIGNAL(modelReset()), this, SLOT(buildMap()));
    connect(sourceModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(sourceModelDataChanged(QModelIndex,QModelIndex)));
    QSortFilterProxyModel::setSourceModel(sourceModel);
    buildMap();
    invalidate();
}

bool N::UniqueValuesProxyModel::isDuplicate(int row) const
{
	QVariant v = sourceModel()->index(row, d->modelColumn).data(filterRole());
	if (!d->emptyValues){
		if (v.toString().isEmpty()){
			return false;
		}
	}
    QMap<QString, QList<int> >::ConstIterator mIt = d->valueMap.constFind(v.toString());
    if (mIt == d->valueMap.constEnd()){
        return true;
    }
    if (mIt.value().first() == row){
        return true;
    }
    return false;
}

void N::UniqueValuesProxyModel::buildMap()
{
    beginResetModel();
    d->valueMap.clear();
    if (sourceModel() == 0){
        return;
    }
    QMap<QString, QList<int> >::Iterator it;
        int c = sourceModel()->rowCount();
    for(int iRow = 0; iRow < c; iRow++){
                QVariant v = sourceModel()->index(iRow, d->modelColumn).data(filterRole());
                it = d->valueMap.find(v.toString());
                if (it == d->valueMap.end()){
                        QList<int> l;
                        l << iRow;
                        d->valueMap[v.toString()] = l;
                } else {
                        it.value().append(iRow);
                }
                emit progressChanged(iRow * 100 / c);
        }
        emit progressChanged(100);
    endResetModel();
    invalidate();
}

void N::UniqueValuesProxyModel::setEmptyItemsAllowed(bool on)
{
	if (d->emptyValues != on){
		buildMap();
	}
}

void N::UniqueValuesProxyModel::sourceModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    buildMap();
}
