#include <qtaiv.h>

N::AdvancedHeaderView:: AdvancedHeaderView (Qt::Orientation orientation,QWidget * parent,Plan * p)
                      : HeaderView         (                orientation,          parent,       p)
{
  setSectionsClickable ( true ) ;
}

N::AdvancedHeaderView::~AdvancedHeaderView(void)
{
}

void N::AdvancedHeaderView::addFilterSetActionTriggered()
{
    QAction* a = qobject_cast<QAction*>(sender());
    if (a){
        model()->insertRows(a->data().toInt(), 1);
    } else {
        model()->insertRows(model()->rowCount(), 1);
    }
}

void N::AdvancedHeaderView::contextMenuEvent(QContextMenuEvent* event)
{
    QAction* a;
    QMenu menu;
    AbstractFilterModel* filterModel = qobject_cast<AbstractFilterModel*>(model());
    if (orientation() == Qt::Horizontal){
        int column = logicalIndexAt(event->pos());
        if (column > -1){
            a = new QAction(QIcon(":/qadvancedtableviews/column.hide"), QString(tr("Hide Column '%1'")).arg(model()->headerData(column, Qt::Horizontal).toString()), &menu);
            connect(a, SIGNAL(triggered()), this, SLOT(hideSectionActionTriggered()));
            a->setData(column);
            menu.addAction(a);
        }
        menu.addAction(tr("Show All Columns"), this, SLOT(showAllColumns()));
        QVector<QAction*> actions(model()->columnCount() < 11?model()->columnCount():10);
        for (int iColumn = 0; iColumn < model()->columnCount() && iColumn < 10; iColumn++){
            a = new QAction(model()->headerData(iColumn, Qt::Horizontal).toString(), &menu);
            a->setCheckable(true);
            a->setChecked(!isSectionHidden(iColumn));
            a->setData(iColumn);
            actions[visualIndex(iColumn)] = a;
            connect(a, SIGNAL(toggled(bool)), this, SLOT(sectionToggled(bool)));
        }
        menu.addSeparator();
        QMenu* columnsMenu = menu.addMenu(QIcon(":/qadvancedtableviews/columns.select"), tr("Columns"));
        for (int iAction = 0; iAction < actions.size(); iAction++){
            columnsMenu->addAction(actions.at(iAction));
        }
        if (model()->columnCount() > 10){
            columnsMenu->addSeparator();
            columnsMenu->addAction(tr("More Columns..."), this, SLOT(moreColumnsActionTriggered()));
        }
        if (filterModel){
            menu.addSeparator();
            QMenu* modeMenu = menu.addMenu(tr("Mode"));
            a = modeMenu->addAction(tr("Filter"), this, SLOT(filterModeActionTriggered()));
            a->setCheckable(true);
            a->setChecked(filterModel->mode() == AdvancedItemViews::FilterMode);

            a = modeMenu->addAction(tr("Highlight"), this, SLOT(highlightModeActionTriggered()));
            a->setCheckable(true);
            a->setChecked(filterModel->mode() == AdvancedItemViews::HighlightMode);

            modeMenu->addSeparator();
            a = modeMenu->addAction(tr("Inverted"), this, SLOT(invertedMatchModeActionTriggered()));
            a->setCheckable(true);
            a->setChecked(filterModel->matchMode() == AdvancedItemViews::MatchInverted);
        }
        FilterView * mvew = qobject_cast<FilterView*>(parentWidget());
        if (mvew != 0){
            menu.addSeparator();
            if (mvew->filterVisible()){
                menu.addAction(tr("Hide Filter"), this, SLOT(hideFilterActionTriggered()));
            } else {
                menu.addAction(tr("Show Filter"), this, SLOT(showFilterActionTriggered()));
            }
        }
    } else if (orientation() == Qt::Vertical){
        int logicalIndex = logicalIndexAt(event->pos());

        a = menu.addAction(QIcon(":/qaiv/filter/add"), tr("Add Filter Set"), this, SLOT(addFilterSetActionTriggered()));
        a->setData(logicalIndex);
        a = menu.addAction(QIcon(":/qaiv/filter/rename"), tr("Rename Filter Set"), this, SLOT(renameFilterActionTriggered()));
        a->setData(logicalIndex);
        //
        a = menu.addAction(QIcon(":/qaiv/filter/delete"), tr("Remove Filter Set"), this, SLOT(removeFilterSetActionTriggered()));
        a->setData(logicalIndex);
        a->setEnabled(model()->rowCount() > 1);
    }
    event->accept();
    menu.exec(mapToGlobal(event->pos()));
}

void N::AdvancedHeaderView::filterModeActionTriggered()
{
    AbstractFilterModel* m = qobject_cast<AbstractFilterModel*>(model());
    if (m){
        m->setMode(AdvancedItemViews::FilterMode);
    }
}

void N::AdvancedHeaderView::hideFilterActionTriggered()
{
    FilterView* v = qobject_cast<FilterView*>(parentWidget());
    if (v){
        v->setFilterVisible(false);
    }
}

void N::AdvancedHeaderView::hideSectionActionTriggered()
{
    QAction* a = qobject_cast<QAction*>(sender());
    if (a){
        setSectionHidden(a->data().toInt(), true);
    }
}

void N::AdvancedHeaderView::highlightModeActionTriggered()
{
    AbstractFilterModel* m = qobject_cast<AbstractFilterModel*>(model());
    if (m){
        m->setMode(AdvancedItemViews::HighlightMode);
    }
}

void N::AdvancedHeaderView::invertedMatchModeActionTriggered()
{
    AbstractFilterModel* m = qobject_cast<AbstractFilterModel*>(model());
    if (m){
        if (m->matchMode() == AdvancedItemViews::MatchNormal){
            m->setMatchMode(AdvancedItemViews::MatchInverted);
        } else {
            m->setMatchMode(AdvancedItemViews::MatchNormal);
        }
    }
}

void N::AdvancedHeaderView::moreColumnsActionTriggered()
{
    QDialog* dlg = new QDialog(this,Qt::Widget);
    dlg->setWindowTitle(tr("More columns..."));
    QVBoxLayout* l = new QVBoxLayout(dlg);

    QTableView* v = new QTableView(dlg);
    l->addWidget(v);

    QDialogButtonBox* b = new QDialogButtonBox(dlg);
    b->setOrientation(Qt::Horizontal);
    b->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    l->addWidget(b);

    QObject::connect(b, SIGNAL(accepted()), dlg, SLOT(accept()));
    QObject::connect(b, SIGNAL(rejected()), dlg, SLOT(reject()));

    QAbstractItemModel* m = model();
    AbstractFilterModel* hp;
    QSortFilterProxyModel* p = qobject_cast<QSortFilterProxyModel*>(model());
    if (p){
        m = p->sourceModel();
        hp = qobject_cast<AbstractFilterModel*>(p);
    }
    QStandardItemModel* cm = new QStandardItemModel(dlg);
    cm->setColumnCount(1);
    cm->setRowCount(m->columnCount());

    v->setModel(cm);
    v->horizontalHeader()->setStretchLastSection(true);
    v->horizontalHeader()->setVisible(false);
    v->verticalHeader()->setVisible(false);

    QStandardItem* item;
    for (int iColumn = 0; iColumn < m->columnCount(); iColumn++){
        item = new QStandardItem(m->headerData(iColumn, Qt::Horizontal).toString());
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        item->setData(isSectionHidden(iColumn)?Qt::Unchecked:Qt::Checked, Qt::CheckStateRole);
        cm->setItem(iColumn, 0, item);
    }
    v->resizeRowsToContents();
    if (dlg->exec()){
        for (int iRow = 0; iRow < cm->rowCount(); iRow++){
            setSectionHidden(iRow, cm->index(iRow, 0).data(Qt::CheckStateRole).toInt() == Qt::Unchecked);
        }
    }
    delete dlg;
}

void N::AdvancedHeaderView::removeFilterSetActionTriggered()
{
    QAction* a = qobject_cast<QAction*>(sender());
    if (a){
        if (QMessageBox::question(this, tr("Remove Filter"), QString(tr("Please confirm to delete filter '%1'.")).arg(model()->headerData(a->data().toInt(), Qt::Vertical, Qt::DisplayRole).toString()), QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes){
            model()->removeRows(a->data().toInt(), 1);
        }
    }
}

void N::AdvancedHeaderView::renameFilterActionTriggered()
{
    QAction* a = qobject_cast<QAction*>(sender());
    if (a){
        QString name = QInputDialog::getText(this, tr("Rename Filter Set"), tr("Name"), QLineEdit::Normal, model()->headerData(a->data().toInt(), Qt::Vertical, Qt::DisplayRole).toString());
        if (!name.isEmpty()){
            model()->setHeaderData(a->data().toInt(), Qt::Vertical, name);
        }
    }
}

void N::AdvancedHeaderView::sectionToggled( bool on )
{
    QAction* a = qobject_cast<QAction*>(sender());
    if (a){
        setSectionHidden(a->data().toInt(), !on);
    }
}

void N::AdvancedHeaderView::showAllColumns()
{
    for (int iSection = 0; iSection < count(); iSection++){
        showSection(iSection);
    }
}

void N::AdvancedHeaderView::showFilterActionTriggered()
{
    FilterView * v = qobject_cast<FilterView*>(parentWidget());
    if (v){
        v->setFilterVisible(true);
    }
}

void N::AdvancedHeaderView::toggleSortOrderActionTriggered()
{
    QAction* a = qobject_cast<QAction*>(sender());
    if (a){
        Qt::SortOrder sortOrder = sortIndicatorOrder();
        if (sortOrder == Qt::AscendingOrder){
            model()->sort(sortIndicatorSection(), Qt::DescendingOrder);
        } else {
            model()->sort(sortIndicatorSection(), Qt::AscendingOrder);
        }
    }
}
