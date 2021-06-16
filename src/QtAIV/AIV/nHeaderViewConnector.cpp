#include <qtaiv.h>

N::HeaderViewConnector:: HeaderViewConnector (Qt::Orientation orientation,QObject * parent)
                       : QObject             (                                      parent)
{
  cAdjustSectionSizePending = true;
  cOrientation = orientation;
}

N::HeaderViewConnector::~HeaderViewConnector(void)
{
}

void N::HeaderViewConnector::append(QHeaderView* view)
{
    if (view->orientation() != cOrientation){
        return;
    }
    cHeaderViewList.append(view);
    connect(view, SIGNAL(destroyed(QObject*)), this, SLOT(viewDestroyed(QObject*)));
    connect(view, SIGNAL(sectionMoved(int,int,int)), this, SLOT(sectionMoved(int,int,int)));
    connect(view, SIGNAL(sectionResized(int,int,int)), this, SLOT(sectionResized(int,int,int)));
    connect(view, SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)), this, SLOT(sortIndicatorChanged(int,Qt::SortOrder)));

    if (cOrientation == Qt::Horizontal){
        QTableView* mView = qobject_cast<QTableView*>(view->parentWidget());
        if (mView){
            connect(mView->horizontalScrollBar(), SIGNAL(rangeChanged(int,int)), this, SLOT(scrollBarRangeChanged(int,int)));
            connect(mView->horizontalScrollBar(), SIGNAL(sliderMoved(int)), this, SLOT(scrollBarSilderMoved(int)));
            connect(mView->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(scrollBarSilderMoved(int)));
        }
    }
}

void N::HeaderViewConnector::adjustSectionSize()
{
    cAdjustSectionSizePending = false;
    int mWidth = 0;
    Q_FOREACH(QHeaderView* mHeaderView, cHeaderViewList){
        if (mHeaderView){
            if (mWidth < mHeaderView->width()){
                mWidth = mHeaderView->width();
            }
        }
    }
    if (mWidth > 0){
        Q_FOREACH(QHeaderView* mHeaderView, cHeaderViewList){
            if (mHeaderView){
                mHeaderView->blockSignals(true);
                mHeaderView->setFixedWidth(mWidth);
                mHeaderView->blockSignals(false);
            }
        }
    }
}

void N::HeaderViewConnector::scrollBarRangeChanged(int min, int max)
{
    if (cOrientation == Qt::Vertical){
        Q_FOREACH(QHeaderView* mHeaderView, cHeaderViewList){
            if (mHeaderView && mHeaderView){
                QTableView* mTableView = qobject_cast<QTableView*>(mHeaderView->parentWidget());
                if (mTableView){
                    if (mTableView->verticalScrollBar()->isVisible()){

                    }
                }
            }
        }
    }
//    QHeaderView* mSender = qobject_cast<QHeaderView*>(sender());
//    Q_FOREACH(QHeaderView* mHeaderView, cHeaderViewList){
//        if (mHeaderView && mHeaderView != mSender){
//            QTableView* mTableView = qobject_cast<QTableView*>(mHeaderView->parentWidget());
//            if (mTableView){
//                if (cOrientation == Qt::Horizontal){
//                    mTableView->horizontalScrollBar()->setRange(min, max);
//                }
//            }
//        }
//    }
}

void N::HeaderViewConnector::scrollBarSilderMoved(int value)
{
    QHeaderView* mSender = qobject_cast<QHeaderView*>(sender());
    Q_FOREACH(QHeaderView* mHeaderView, cHeaderViewList){
        if (mHeaderView && mHeaderView != mSender){
            QTableView* mTableView = qobject_cast<QTableView*>(mHeaderView->parentWidget());
            if (mTableView){
                if (cOrientation == Qt::Horizontal){
                    mTableView->horizontalScrollBar()->setValue(value);
                }
            }
        }
    }
}

void N::HeaderViewConnector::sectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex)
{
    Q_UNUSED(logicalIndex);
    QHeaderView* mSender = qobject_cast<QHeaderView*>(sender());
    Q_FOREACH(QHeaderView* mHeaderView, cHeaderViewList){
        if (mHeaderView && mHeaderView != mSender){
            mHeaderView->blockSignals(true);
            mHeaderView->moveSection(oldVisualIndex, newVisualIndex);
            mHeaderView->blockSignals(false);
        }
    }
}

void N::HeaderViewConnector::sectionResized(int logicalIndex, int oldSize, int newSize)
{
    Q_UNUSED(oldSize);
    QHeaderView* mSender = qobject_cast<QHeaderView*>(sender());
    Q_FOREACH(QHeaderView* mHeaderView, cHeaderViewList){
        if (mHeaderView && mHeaderView != mSender){
            mHeaderView->resizeSection(logicalIndex, newSize);
        }
    }
    if (cAdjustSectionSizePending){
        adjustSectionSize();
    }
}

void N::HeaderViewConnector::sortIndicatorChanged(int logicalIndex, Qt::SortOrder order)
{
    QHeaderView* mSender = qobject_cast<QHeaderView*>(sender());
    Q_FOREACH(QHeaderView* mHeaderView, cHeaderViewList){
        if (mHeaderView && mHeaderView != mSender){
            mHeaderView->blockSignals(true);
            mHeaderView->setSortIndicator(logicalIndex, order);
            mHeaderView->blockSignals(false);
        }
    }
}

void N::HeaderViewConnector::viewDestroyed(QObject* object)
{
    cHeaderViewList.removeAt(cHeaderViewList.indexOf(qobject_cast<QHeaderView*>(object)));
}
