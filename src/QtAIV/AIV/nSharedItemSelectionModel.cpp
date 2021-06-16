#include "nAivPrivate.hpp"

#define d ((SharedItemSelectionModelPrivate *)D)

N::SharedItemSelectionModel:: SharedItemSelectionModel       (
                                QAbstractItemModel  * model  ,
                                QItemSelectionModel * shared ,
                                QObject             * parent )
                            : QItemSelectionModel(model, parent)
                            , D((void *)new SharedItemSelectionModelPrivate(this))
{
  d->shared = shared;
  connect(d->shared, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(sharedSelectionChanged(QItemSelection,QItemSelection)));
}

N::SharedItemSelectionModel::~SharedItemSelectionModel(void)
{
  delete d ;
  D = NULL ;
}

void N::SharedItemSelectionModel::select(const QModelIndex &index, QItemSelectionModel::SelectionFlags command)
{
    QItemSelectionModel::select(index, command);
    d->shared->select(index, command);
}

void N::SharedItemSelectionModel::select(const QItemSelection &selection, QItemSelectionModel::SelectionFlags command)
{
    QItemSelectionModel::select(selection, command);
    d->shared->select(selection, command);
}

void N::SharedItemSelectionModel::sharedSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    QItemSelectionModel::select(selected, QItemSelectionModel::Select);
    QItemSelectionModel::select(deselected, QItemSelectionModel::Deselect);
}

