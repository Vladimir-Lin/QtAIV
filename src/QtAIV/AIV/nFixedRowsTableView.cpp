#include "nAivPrivate.hpp"

#define d ((FixedRowsTableViewPrivate *)D)

N::FixedRowsTableView:: FixedRowsTableView (QWidget * parent,Plan * p)
                      : TableView          (          parent,       p)
                      , D((void *)new FixedRowsTableViewPrivate(this))
{
    d->filterProxy     = new FixedRowsFilterProxyModel     (this                ) ;
    d->decorationProxy = new FixedRowsDecorationProxyModel (d->filterProxy, this) ;

    connect(verticalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(verticalHeaderSectionClicked(int)));

    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    QTableView::setModel(d->filterProxy);

    connect(d->filterProxy, SIGNAL(layoutChanged()), this, SLOT(updateHeight()));
    connect(d->filterProxy, SIGNAL(modelReset()), this, SLOT(updateHeight()));
    connect(d->filterProxy, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(updateHeight()));
    connect(d->filterProxy, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(updateHeight()));

    connect(d->decorationProxy, SIGNAL(modelToggled(bool)), d->filterProxy, SLOT(setEnabled(bool)));

    updateHeight();
}

N::FixedRowsTableView::~FixedRowsTableView(void)
{
  delete d ;
  D = NULL ;
}

N::FixedRowsDecorationProxyModel * N::FixedRowsTableView::decorationProxy() const
{
    return d->decorationProxy;
}

N::FixedRowsFilterProxyModel * N::FixedRowsTableView::filterProxy() const
{
    return d->filterProxy;
}

void N::FixedRowsTableView::setModel(QAbstractItemModel *model)
{
    d->decorationProxy->setSourceModel(model);
    d->filterProxy->setSourceModel(model);

    connect(model, SIGNAL(modelReset()), d->filterProxy, SLOT(sourceModelReset()));
    updateHeight();
}

void N::FixedRowsTableView::closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint)
{
    if (editor->parent()->parent() == this){
        QTableView::closeEditor(editor, hint);
    } else {
        qDebug() << "closeEditor(...) ignored by" << this;
    }
}

bool N::FixedRowsTableView::fixedRowsMode() const
{
        return d->filterProxy->isEnabled();
}

void N::FixedRowsTableView::focusInEvent(QFocusEvent *event)
{
    QTableView::focusInEvent(event);
    emit focusReceived();
}

void N::FixedRowsTableView::setFixedRowsMode(bool on)
{
        d->filterProxy->setEnabled(on);
}

void N::FixedRowsTableView::updateHeight()
{
    int h = 0;
    if (model() == 0 || model()->rowCount() == 0){
        hide();
    } else {
        resizeRowsToContents();
        show();
        for (int i = 0; i < model()->rowCount(); i++){
            h += rowHeight(i);
        }
        h += 2;
    }
    setFixedHeight(h);
}

void N::FixedRowsTableView::verticalHeaderSectionClicked(int section)
{
    QModelIndex i = model()->index(section, 0);
    QAbstractProxyModel* proxy;
    while((proxy = qobject_cast<QAbstractProxyModel*>((QAbstractProxyModel*)i.model()))){
        i = proxy->mapToSource(i);
    }
    d->filterProxy->toggleRow(i);
}

