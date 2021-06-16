#include "nAivPrivate.hpp"

N::FilterViewConnector:: FilterViewConnector(FilterView * filterView) :
    QObject(filterView)
{
    cFilterView = filterView;
    cTableView = 0;
    cTreeView = 0;

    connect(cFilterView->horizontalHeader(), SIGNAL(sectionMoved(int,int,int)), this, SLOT(filterViewHorizontalSectionMoved(int,int,int)));
    connect(cFilterView->horizontalHeader(), SIGNAL(sectionResized(int,int,int)), this, SLOT(filterViewHorizontalSectionResized(int,int,int)));
    connect(cFilterView->horizontalHeader(), SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)), this, SLOT(filterViewHorizontalSortIndicatorChanged(int,Qt::SortOrder)));
}

N::FilterViewConnector:: FilterViewConnector(FilterView * filterView, QTableView* tableView, QObject* parent) :
    QObject(parent)
{
    cFilterView = filterView;
    cTableView = tableView;
    cTreeView = 0;

    connect(cFilterView->horizontalHeader(), SIGNAL(sectionMoved(int,int,int)), this, SLOT(filterViewHorizontalSectionMoved(int,int,int)));
    connect(cFilterView->horizontalHeader(), SIGNAL(sectionResized(int,int,int)), this, SLOT(filterViewHorizontalSectionResized(int,int,int)));
    connect(cFilterView->horizontalHeader(), SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)), this, SLOT(filterViewHorizontalSortIndicatorChanged(int,Qt::SortOrder)));

    setDataView(tableView);
}

N::FilterViewConnector:: FilterViewConnector(FilterView * filterView, QTreeView* treeView, QObject* parent) :
    QObject(parent)
{
    cFilterView = filterView;
    cTableView = 0;
    cTreeView = treeView;

    connect(cFilterView->horizontalHeader(), SIGNAL(sectionMoved(int,int,int)), this, SLOT(filterViewHorizontalSectionMoved(int,int,int)));
    connect(cFilterView->horizontalHeader(), SIGNAL(sectionResized(int,int,int)), this, SLOT(filterViewHorizontalSectionResized(int,int,int)));
    connect(cFilterView->horizontalHeader(), SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)), this, SLOT(filterViewHorizontalSortIndicatorChanged(int,Qt::SortOrder)));

    setDataView(treeView);
}

N::FilterViewConnector::~FilterViewConnector(void)
{
}

void N::FilterViewConnector::setDataView(QTableView *view)
{
    if (cTableView){
        disconnect(cFilterView->horizontalHeader(), 0, this, 0);
        disconnect(cTableView->horizontalScrollBar(), 0, this, 0);
        disconnect(cTableView->verticalHeader(), 0, this, 0);
    }
    cTableView = view;
    connect(cTableView->horizontalHeader(), SIGNAL(sectionMoved(int,int,int)), this, SLOT(dataViewHorizontalSectionMoved(int,int,int)));
    connect(cTableView->horizontalHeader(), SIGNAL(sectionResized(int,int,int)), this, SLOT(dataViewHorizontalSectionResized(int,int,int)));
    connect(cTableView->horizontalHeader(), SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)), this, SLOT(dataViewHorizontalSortIndicatorChanged(int,Qt::SortOrder)));
    connect(cTableView->horizontalScrollBar(), SIGNAL(rangeChanged(int,int)), this, SLOT(dataViewHorizontalScrollBarRangeChanged(int,int)));
    connect(cTableView->horizontalScrollBar(), SIGNAL(sliderMoved(int)), this, SLOT(dataViewHorizontalScrollBarSilderMoved(int)));

    connect(cTableView->verticalHeader(), SIGNAL(sectionCountChanged(int,int)), this, SLOT(adjustVerticalHeaderWidth()));
    connect(cTableView->verticalScrollBar(), SIGNAL(rangeChanged(int,int)), this, SLOT(dataViewVerticalScrollBarRangeChanged(int,int)));
}

void N::FilterViewConnector::setDataView(QTreeView *view)
{
    if (cTreeView){
        disconnect(cTreeView->header(), 0, this, 0);
    }
    cTreeView = view;
    connect(cTreeView->header(), SIGNAL(sectionMoved(int,int,int)), this, SLOT(dataViewHorizontalSectionMoved(int,int,int)));
    connect(cTreeView->header(), SIGNAL(sectionResized(int,int,int)), this, SLOT(dataViewHorizontalSectionResized(int,int,int)));
    connect(cTreeView->header(), SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)), this, SLOT(dataViewHorizontalSortIndicatorChanged(int,Qt::SortOrder)));
    connect(cTreeView->header(), SIGNAL(rangeChanged(int,int)), this, SLOT(dataViewHorizontalScrollBarRangeChanged(int,int)));
    connect(cTreeView->header(), SIGNAL(sliderMoved(int)), this, SLOT(dataViewHorizontalScrollBarSilderMoved(int)));
}

void N::FilterViewConnector::adjustVerticalHeaderWidth()
{
    if (cTableView){
        if (cTableView->verticalHeader()->width() < cFilterView->verticalHeader()->width()){
            cTableView->verticalHeader()->blockSignals(true);
            cTableView->verticalHeader()->setFixedWidth(cFilterView->verticalHeader()->width());
            cTableView->verticalHeader()->blockSignals(false);
        } else {
            cFilterView->verticalHeader()->blockSignals(true);
            cFilterView->verticalHeader()->setFixedWidth(cTableView->verticalHeader()->width());
            cFilterView->verticalHeader()->blockSignals(false);

        }
        if (cFilterView->model()->rowCount() > cFilterView->maxVisibileFilterSets()){
            cTableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        }
    }
}

void N::FilterViewConnector::dataViewHorizontalScrollBarRangeChanged(int min, int max)
{
    cFilterView->horizontalScrollBar()->setRange(min, max);
}

void N::FilterViewConnector::dataViewHorizontalScrollBarSilderMoved(int value)
{
    cFilterView->horizontalScrollBar()->setValue(value);
}

void N::FilterViewConnector::dataViewHorizontalSortIndicatorChanged(int logicalIndex, Qt::SortOrder order)
{
    cFilterView->horizontalHeader()->blockSignals(true);
    cFilterView->sortByColumn(logicalIndex, order);
    cFilterView->horizontalHeader()->blockSignals(false);
}

void N::FilterViewConnector::dataViewHorizontalSectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex)
{
    Q_UNUSED(logicalIndex);
    cFilterView->horizontalHeader()->moveSection(oldVisualIndex, newVisualIndex);
}

void N::FilterViewConnector::dataViewHorizontalSectionResized(int logicalIndex, int oldSize, int newSize)
{
    Q_UNUSED(oldSize);
    cFilterView->horizontalHeader()->resizeSection(logicalIndex, newSize);
}

void N::FilterViewConnector::dataViewVerticalScrollBarRangeChanged(int min, int max)
{
    Q_UNUSED(min);
    Q_UNUSED(max);
    if (cTableView){
        if (cTableView->verticalScrollBar()->maximum() == 0){
            cFilterView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        } else {
            cFilterView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        }
    } else if (cTreeView){
        if (cTreeView->verticalScrollBar()->maximum() == 0){
            cFilterView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        } else {
            cFilterView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        }
    }

}

void N::FilterViewConnector::filterViewHorizontalSectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex)
{
    Q_UNUSED(logicalIndex);
    if (cTableView){
        cTableView->horizontalHeader()->moveSection(oldVisualIndex, newVisualIndex);
    } else if (cTreeView){
        cTreeView->header()->moveSection(oldVisualIndex, newVisualIndex);
    }
}

void N::FilterViewConnector::filterViewHorizontalSectionResized(int logicalIndex, int oldSize, int newSize)
{
    Q_UNUSED(oldSize);
    if (cTableView){
        cTableView->horizontalHeader()->resizeSection(logicalIndex, newSize);
    } else if (cTreeView){
        cTreeView->header()->resizeSection(logicalIndex, newSize);
    }
}

void N::FilterViewConnector::filterViewHorizontalSortIndicatorChanged(int logicalIndex, Qt::SortOrder order)
{
    if (cTableView){
        cTableView->sortByColumn(logicalIndex, order);
    } else if (cTreeView){
        cTreeView->sortByColumn(logicalIndex, order);
    }
}
