#include "nAivPrivate.hpp"

#define d ((FixedRowsFilterProxyModelPrivate *)D)

N::FixedRowsFilterProxyModel:: FixedRowsFilterProxyModel (QObject * parent)
                             : QSortFilterProxyModel     (          parent)
                             , D((void *)new FixedRowsFilterProxyModelPrivate(this))
{
    d->enabled = false;
}

N::FixedRowsFilterProxyModel::~FixedRowsFilterProxyModel(void)
{
  delete d ;
  D = NULL ;
}

void N::FixedRowsFilterProxyModel::clear()
{
    d->rows.clear();
    invalidateFilter();
}

bool N::FixedRowsFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent);
    if (sourceModel() == 0 || !d->enabled){
        return false;
    }
    return d->rows.contains(QPersistentModelIndex(sourceModel()->index(source_row, 0)));
}

QVariant N::FixedRowsFilterProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical && role == Qt::DecorationRole){
        QIcon state;
        state = QIcon(":/qaiv/pin/fixed");
        return state;
    }
    return QSortFilterProxyModel::headerData(section, orientation, role);
}

bool N::FixedRowsFilterProxyModel::isEnabled() const
{
    return d->enabled;
}

bool N::FixedRowsFilterProxyModel::isRowPinned(int row) const
{
    if (sourceModel() == 0 || !d->enabled){
        return false;
    }
    return d->rows.contains(QPersistentModelIndex(sourceModel()->index(row, 0)));
}

void N::FixedRowsFilterProxyModel::setEnabled(bool on)
{
    if (d->enabled != on){
        d->enabled = on;
        invalidateFilter();
    }
}

void N::FixedRowsFilterProxyModel::sourceModelReset()
{
	/**
	 * @todo slot sourceModelReset() should not be public
	 */
    d->rows.clear();
    invalidateFilter();
}

void N::FixedRowsFilterProxyModel::toggleRow(const QModelIndex &index)
{
    QPersistentModelIndex i(index);
    if (!d->rows.contains(i)){
        d->rows.append(i);
    } else {
        d->rows.removeAt(d->rows.indexOf(i));
    }
    invalidateFilter();
}

void N::FixedRowsFilterProxyModel::setRowFixed(const QModelIndex &index, bool fixed)
{
    if (fixed){

    } else {

    }
}
