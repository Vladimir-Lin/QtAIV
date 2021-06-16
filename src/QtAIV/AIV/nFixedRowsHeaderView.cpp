#include <qtaiv.h>

N::FixedRowsHeaderView:: FixedRowsHeaderView (Qt::Orientation orientation,QWidget * parent,Plan * p)
                       : HeaderView          (                orientation,          parent,       p)
{
}

N::FixedRowsHeaderView::~FixedRowsHeaderView(void)
{
}

void N::FixedRowsHeaderView::mouseReleaseEvent(QMouseEvent * event)
{
//    if (event->button() == Qt::LeftButton && orientation() == Qt::Vertical && event->pos().x() < 32){
//        qDebug() << "toggle";
//    } else {
        QHeaderView::mouseReleaseEvent(event);
//    }
}
