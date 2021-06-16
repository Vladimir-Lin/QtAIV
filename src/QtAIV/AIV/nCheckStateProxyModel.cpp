#include "nAivPrivate.hpp"

#define d ((CheckStateProxyModelPrivate *)D)

N::CheckStateProxyModel:: CheckStateProxyModel(QObject * parent)
                        : QIdentityProxyModel(parent)
                        , D((void *)new CheckStateProxyModelPrivate(this))
{
}

N::CheckStateProxyModel::~CheckStateProxyModel(void)
{
    delete d;
}

int N::CheckStateProxyModel::checkableColumnsCount() const
{
        return d->columns.size();
}

QModelIndexList N::CheckStateProxyModel::checkedIndexes() const
{
    return d->checkedIndexes;
}

QVariant N::CheckStateProxyModel::data(const QModelIndex & index, int role) const
{
    if (role == Qt::CheckStateRole && d->columns.contains(index.column())){
        return d->checkedIndexes.contains(index)?Qt::Checked:Qt::Unchecked;
    }
        return QIdentityProxyModel::data(index, role);
}

Qt::ItemFlags N::CheckStateProxyModel::flags(const QModelIndex & index) const
{
        Qt::ItemFlags f = QIdentityProxyModel::flags(index);
    if (index.isValid() && d->columns.contains(index.column())){
        f |= Qt::ItemIsUserCheckable;
    }
    return f;
}

bool N::CheckStateProxyModel::isChecked(const QModelIndex & index) const
{
        return d->checkedIndexes.contains(index);
}

bool N::CheckStateProxyModel::isColumnCheckable(int column) const
{
    return d->columns.contains(column);
}

void N::CheckStateProxyModel::setAllChecked(bool checked)
{
	if (rowCount() == 0 || columnCount() == 0){
		return;
	}
	if (checked){
		for (int c = 0; c < d->columns.size(); c++){
			for (int r = 0; r < rowCount(); r++){
				QModelIndex i = index(r, c);
				if (!d->checkedIndexes.contains(i)){
					d->checkedIndexes.append(i);
				}
			}
		}
	} else {
		d->checkedIndexes.clear();
	}
	emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
}

void N::CheckStateProxyModel::setColumnCheckable(int column, bool checkable)
{
	//if (column < columnCount()){
		if (checkable){
			if (!d->columns.contains(column)){
				d->columns << column;
			}
		} else {
			d->columns.removeAt(d->columns.indexOf(column));
		}
	//}
}

void N::CheckStateProxyModel::setChecked(const QModelIndex & index, bool checked)
{
	if (d->columns.contains(index.column()) && index.row() < rowCount()){
		if (checked){
			d->checkedIndexes.append(index);
			emit dataChanged(index, index);
		} else {
			if (d->checkedIndexes.contains(index)){
				d->checkedIndexes.removeAt(d->checkedIndexes.indexOf(index));
				emit dataChanged(index, index);
			}
		}
	}
}

void N::CheckStateProxyModel::setCheckedIndexes(const QModelIndexList & indexes)
{
        beginResetModel();
    d->checkedIndexes = indexes;
        endResetModel();
}

void N::CheckStateProxyModel::setCheckedValues(int column, const QVariantList & values)
{
	if (sourceModel() == 0){
		return;
	}
	if (column < columnCount()){
		d->checkedIndexes.clear();
		QModelIndex mIndex;
		for(int iRow = 0; iRow < sourceModel()->rowCount(); iRow++){
			mIndex = mapFromSource(sourceModel()->index(iRow, column));
			if (values.contains(mIndex.data())){
				d->checkedIndexes.append(mIndex);
			}
		}
		if (!d->checkedIndexes.isEmpty()){
			emit dataChanged(d->checkedIndexes.first(), d->checkedIndexes.last());
		}
	}
}

bool N::CheckStateProxyModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if (index.row() >= rowCount() || index.column() >= columnCount()){
		return false;
	}
    if (role == Qt::CheckStateRole){
        if (value.toInt() == Qt::Checked){
            d->checkedIndexes.append(index);
        } else {
            d->checkedIndexes.removeAt(d->checkedIndexes.indexOf(index));
        }
        emit dataChanged(index, index);
        return true;
    }
        return QIdentityProxyModel::setData(index, value, role);
}

void N::CheckStateProxyModel::setSourceModel(QAbstractItemModel* sourceModel)
{
	beginResetModel();
	if (sourceModel){
		if (N::CheckStateProxyModel::sourceModel()){
			disconnect(N::CheckStateProxyModel::sourceModel(), 0, this, 0);
		}
		connect(sourceModel, SIGNAL(modelAboutToBeReset()), this, SLOT(sourceModelAboutToBeReset()));
	}
	QIdentityProxyModel::setSourceModel(sourceModel);

	d->checkedIndexes.clear();
	d->columns.clear();

	endResetModel();
}

void N::CheckStateProxyModel::sourceModelAboutToBeReset()
{
    d->checkedIndexes.clear();
}

void N::CheckStateProxyModel::sourceModelRowsAboutToBeRemoved(const QModelIndex & parent, int start, int end)
{
	for (int i = start; i <= end; i++){
		QListIterator<QModelIndex> it(d->checkedIndexes);
		while(it.hasNext()){
			QModelIndex index = it.next();
			if (index.row() == i){
				d->checkedIndexes.removeAt(d->checkedIndexes.indexOf(index));
			}
		}
	}
}
