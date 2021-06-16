#include "nAivPrivate.hpp"

#define d ((FilterViewPrivate *)D)

N::FilterView:: FilterView (QWidget * parent,Plan * p)
              : TableView  (          parent,       p)
              , D((void *)new FilterViewPrivate(this))
{
    setItemDelegate(new FilterViewItemDelegate(this));

    setHorizontalHeader(new AdvancedHeaderView(Qt::Horizontal, this));
    setVerticalHeader(new AdvancedHeaderView(Qt::Vertical, this));

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QList<QAbstractButton *> mCornerButtons = this->findChildren<QAbstractButton*>() ;
    QAbstractButton* mCornerButton = NULL ;
    if (mCornerButtons.count()>0) mCornerButton = mCornerButtons[0] ;
    if (mCornerButton){
        mCornerButton->disconnect();
        connect(mCornerButton, SIGNAL(clicked()), this, SIGNAL(cornerButtonClicked()));
    }
    updateGeometry();

}

N::FilterView::~FilterView(void)
{
  delete d ;
  D = NULL ;
}

void N::FilterView::connectToView(QAbstractItemView* view)
{
    QTableView* tableView = qobject_cast<QTableView*>(view);
    if (tableView){
        d->filterViewConnector->setDataView(tableView);
    }
    QTreeView* treeView = qobject_cast<QTreeView*>(view);
    if (treeView){
        d->filterViewConnector->setDataView(treeView);
    }
}

void N::FilterView::addFilter()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (action){
        QModelIndex index = model()->index(action->data().toMap().value("row").toInt(), action->data().toMap().value("column").toInt());
        if (index.isValid()){
            model()->setData(index, action->data().toMap());
            edit(index);
        }
    }
}

void N::FilterView::changeColor()
{
    QVariantMap properties = currentIndex().data(Qt::EditRole).toMap();
    if (!properties.isEmpty()){
        QColor color = qvariant_cast<QColor>(properties.value("highlightColor"));
        color = QColorDialog::getColor(color, this, tr("Select Highlight Color"));
        if (color.isValid()){
            properties["highlightColor"] = color;
            model()->setData(currentIndex(), properties);
        }
    }
}

void N::FilterView::changeProperties()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (action){
        QModelIndex index = model()->index(action->data().toMap().value("row").toInt(), action->data().toMap().value("column").toInt());
        if (index.isValid()){
            model()->setData(index, action->data().toMap());
        }
    }
}

void N::FilterView::disableSelectedFilters()
{
    QVariantMap properties;
    Q_FOREACH(QModelIndex index, selectionModel()->selectedIndexes()){
        properties = index.data(Qt::EditRole).toMap();
        if (!properties.isEmpty()){
            properties["enabled"] = false;
            model()->setData(index, properties);
        }
    }
}

void N::FilterView::enableSelectedFilters()
{
    QVariantMap properties;
    Q_FOREACH(QModelIndex index, selectionModel()->selectedIndexes()){
        properties = index.data(Qt::EditRole).toMap();
        if (!properties.isEmpty()){
            properties["enabled"] = true;
            model()->setData(index, properties);
        }
    }
}

void N::FilterView::contextMenuEvent(QContextMenuEvent* event)
{
    AbstractFilterModel* m = qobject_cast<AbstractFilterModel*>(model());
    if (m == 0){
        event->ignore();
        return;
    }
    if (selectedIndexes().isEmpty()){
        event->ignore();
        return;
    }
    QAction* action = 0;
    QMenu* menu = new QMenu(this);

    if (selectedIndexes().size() > 1){
        menu->addAction(QIcon(":/qaiv/filter/enabled"), tr("Enable Filter"), this, SLOT(enableSelectedFilters()));
        menu->addAction(QIcon(":/qaiv/filter/disabled"), tr("Disable Filter"), this, SLOT(disableSelectedFilters()));
        menu->addSeparator();
        menu->addAction(QIcon(":/qaiv/filter/toggle"), tr("Toggle Filter"), this, SLOT(toggleSelectedFilters()));
    } else {
        QVariantMap properties = selectedIndexes().first().data(Qt::EditRole).toMap();
        if (properties.isEmpty()){
            properties["row"] = selectedIndexes().first().row();
            properties["column"] = selectedIndexes().first().column();
            properties["enabled"] = true;
            QVariantList mTypes = selectedIndexes().first().data(AbstractFilterModel::ColumnFilterTypesRole).toList();
            Q_FOREACH(AbstractFilterModel::FilterTypeEntry entry , m->registeredFilterTypes()){
                if (entry.type == AbstractFilter::Type){
                    menu->addSeparator();
                } else{
                    if (mTypes.isEmpty() || mTypes.contains(entry.type)){
                        action = menu->addAction(entry.icon, entry.text, this, SLOT(addFilter()));
                        properties["type"] = entry.type;
                        action->setData(properties);
                        action->setToolTip(entry.tooltip);
                    }
                }
            }
        } else {
            m->filter(selectedIndexes().first())->addContextMenuActions(menu, this);
            if (!menu->isEmpty()){
                menu->addSeparator();
            }
            if (properties.value("enabled").toBool()){
                action = menu->addAction(QIcon(":/qaiv/filter/disabled"), tr("Disable Filter"), this, SLOT(changeProperties()));
                properties["enabled"] = false;
            } else {
                action = menu->addAction(QIcon(":/qaiv/filter/enabled"), tr("Enable Filter"), this, SLOT(changeProperties()));
                properties["enabled"] = true;
            }
            action->setData(properties);
            if (m->mode() == AdvancedItemViews::HighlightMode){
                menu->addSeparator();
                QPixmap pixmap(22, 22);
                pixmap.fill(qvariant_cast<QColor>(properties.value("highlightColor")));
                action = menu->addAction(tr("Change Color..."), this, SLOT(changeColor()));
                action->setIcon(QIcon(pixmap));
                action->setData(properties);
            }
            menu->addSeparator();
            action = menu->addAction(tr("Remove filter"), this, SLOT(removeFilter()));
            action->setData(properties);
        }
    }

    //        QMenu* mMenu = mModel->createStandardContextMenu(selectionModel()->selectedIndexes(), this);
    menu->exec(viewport()->mapToGlobal(event->pos()));
    event->accept();
    delete menu;
}

void N::FilterView::focusInEvent(QFocusEvent *event)
{
    QTableView::focusInEvent(event);
    emit focusReceived();
}

int N::FilterView::defaultFilterType() const
{
    return d->defaultFilterType;
}

N::FilterViewConnector * N::FilterView::filterViewConnector() const
{
    return d->filterViewConnector;
}

bool N::FilterView::filterVisible() const
{
    return d->filterVisible;
}

int N::FilterView::maxVisibileFilterSets() const
{
    return d->maxFilterVisible;
}

void N::FilterView::mousePressEvent( QMouseEvent* event )
{
    QModelIndex index = indexAt(event->pos());
    if (event->button() == Qt::LeftButton && index.isValid()){
        QRect r = visualRect(index);
        r.setWidth(16);
        if (r.contains(event->pos())){
                        toggleFilter(index);
        }
                QTableView::mousePressEvent(event);
    } else {
        QTableView::mousePressEvent(event);
    }
}

void N::FilterView::hideFilter()
{
    setFilterVisible(false);
}

void N::FilterView::setDefaultFilterType(int type)
{
    d->defaultFilterType = type;
}

void N::FilterView::setMaxVisibileFilterSets(int rows)
{
    d->maxFilterVisible = rows;
    emit calcGeometryRequested();
}

void N::FilterView::setModel(QAbstractItemModel* model)
{
    QTableView::setModel(model);
    connect(model, SIGNAL(modelReset()), this, SIGNAL(calcGeometryRequested()));
    connect(model, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SIGNAL(calcGeometryRequested()));
    connect(model, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SIGNAL(calcGeometryRequested()));
//    calcGeometry();
    updateGeometry();
}

void N::FilterView::showFilter()
{
    setFilterVisible(true);
}

void N::FilterView::updateGeometry()
{
    if (model() == 0){
        setFixedHeight(horizontalHeader()->sizeHint().height());
    } else {
        int rows = model()->rowCount();
        if (rows > maxVisibileFilterSets()){
            rows = maxVisibileFilterSets();
        }
        for (int iRow = 0; iRow < model()->rowCount(); iRow++){
            verticalHeader()->resizeSection(iRow, verticalHeader()->sizeHint().height());
        }
        int rowHeight = verticalHeader()->sizeHint().height();
        if (rowHeight == 0){
            rowHeight = horizontalHeader()->sizeHint().height();
        }
        int headerHeight = horizontalHeader()->sizeHint().height();
        if (headerHeight == 0){
            headerHeight = rowHeight;
        }
        if (filterVisible()){
            setFixedHeight(headerHeight + rowHeight * rows + (1 * rows) + 1);
        } else {
            setFixedHeight(horizontalHeader()->sizeHint().height());
        }
    }
}

void N::FilterView::removeFilter()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (action){
        QModelIndex index = model()->index(action->data().toMap().value("row").toInt(), action->data().toMap().value("column").toInt());
        if (index.isValid()){
            model()->setData(index, QVariantMap());
        }
    }
}

void N::FilterView::setFilterVisible(bool visible)
{
    if (visible != d->filterVisible){
        d->filterVisible = visible;
        emit visibilityChanged(d->filterVisible);
    }
}

void N::FilterView::toggleFilter(const QModelIndex & index)
{
	if (index.isValid()){
	    QVariantMap p;
	p = index.data(Qt::EditRole).toMap();
	if (!p.isEmpty()){
	    p["enabled"] = !p.value("enabled").toBool();
	    model()->setData(index, p);
	}
	}
}

void N::FilterView::toggleSelectedFilters()
{
    QVariantMap properties;
    Q_FOREACH(QModelIndex index, selectionModel()->selectedIndexes()){
        properties = index.data(Qt::EditRole).toMap();
        if (!properties.isEmpty()){
            properties["enabled"] = !properties.value("enabled").toBool();
            model()->setData(index, properties);
        }
    }
}
