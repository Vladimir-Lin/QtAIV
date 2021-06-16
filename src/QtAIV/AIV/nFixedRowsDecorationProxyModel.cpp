#include "nAivPrivate.hpp"

#define d ((FixedRowsDecorationProxyModelPrivate *)D)

N::FixedRowsDecorationProxyModel:: FixedRowsDecorationProxyModel(FixedRowsFilterProxyModel *filterProxy, QObject *parent)
                                 : QIdentityProxyModel          (parent)
                                 , D((void *)new FixedRowsDecorationProxyModelPrivate(this))
{
    d->enabled = false;
    d->filterProxy = filterProxy;
}

N::FixedRowsDecorationProxyModel::~FixedRowsDecorationProxyModel(void)
{
  delete d ;
  D = NULL ;
}

QVariant N::FixedRowsDecorationProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (d->enabled && orientation == Qt::Vertical && role == Qt::DecorationRole){
        if (d->filterProxy){
            QIcon icon = qvariant_cast<QIcon>(QIdentityProxyModel::headerData(section, orientation, role));
            QIcon state;
            if (d->filterProxy->isRowPinned(section)){
                state = QIcon(":/qaiv/pin/fixed");
            } else {
                state = QIcon(":/qaiv/pin/free");
            }
            if (icon.isNull()){
                return state;
            }
        }
    }
    return QIdentityProxyModel::headerData(section, orientation, role);
}

QSize N::FixedRowsDecorationProxyModel::iconSize() const
{
        return QSize(24, 24);
}

bool N::FixedRowsDecorationProxyModel::isEnabled() const
{
    return d->enabled;
}

void N::FixedRowsDecorationProxyModel::setEnabled(bool on)
{
    if (d->enabled != on){
        emit layoutAboutToBeChanged();
        d->enabled = on;
        emit modelToggled(d->enabled);
        emit layoutChanged();
    }
}

void N::FixedRowsDecorationProxyModel::toggleRow(const QModelIndex & index)
{
	QModelIndex i(index);
	QAbstractProxyModel* p;
	while(i.model() != sourceModel() && (p = qobject_cast<QAbstractProxyModel*>((QAbstractProxyModel*)i.model()))){
	i = p->mapToSource(i);
    }
    d->filterProxy->toggleRow(i);
}
