#include <qtaiv.h>

N::RangeFilterEditor:: RangeFilterEditor  ( QWidget * parent , Plan * p )
                     : FilterEditorWidget (           parent ,        p )
{
  setPopup(new RangeFilterEditorPopup(this));
}

N::RangeFilterEditor::~RangeFilterEditor(void)
{
}

bool N::RangeFilterEditor::eventFilter(QObject *obj, QEvent *event)
{
        if (event->type() == QEvent::KeyPress){
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
                if (keyEvent->key() == Qt::Key_Return){
                        emit commitAndClose();
                        return true;
                } else {
                        return QObject::eventFilter(obj, event);
                }
        } else {
                return QObject::eventFilter(obj, event);
        }
}
