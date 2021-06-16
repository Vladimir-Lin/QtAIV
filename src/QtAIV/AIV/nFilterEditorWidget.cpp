#include <qtaiv.h>

N::FilterEditorWidget:: FilterEditorWidget (QWidget * parent,Plan * p)
                      : Widget             (          parent,       p)
{
  m_popup = 0;
}

N::FilterEditorWidget::~FilterEditorWidget(void)
{
}

bool N::FilterEditorWidget::eventFilter(QObject* object, QEvent* event)
{
	if (m_popup){
		if (event->type() == QEvent::Hide){
			emit commitAndClose();
			return true;
		} else if ( event->type() == QEvent::KeyPress){
			QKeyEvent* keyEvent = static_cast<QKeyEvent *>(event);
			if (popup()->cancelAndClose(object, keyEvent->key())){
				emit cancelAndClose();
				return true;
			}
			if (popup()->commitAndClose(object, keyEvent->key())){
				emit commitAndClose();
				return true;
			}
		}
	}
	return QObject::eventFilter(object, event);
}

void N::FilterEditorWidget::moveEvent(QMoveEvent* e)
{
	QWidget::moveEvent(e);
	if (m_popup){
		movePopup();
	}
}

void N::FilterEditorWidget::movePopup()
{
	if (m_popup){
		QPoint p;
		if (m_popup->geometry().height() + mapToGlobal(pos()).y() > QApplication::desktop()->availableGeometry(this).height()){
			p = mapToGlobal(rect().topLeft());
			p.setY(p.y() - m_popup->geometry().height());
		} else {
			p = mapToGlobal(rect().bottomLeft());
		}
		m_popup->move(p);
	}
}

N::FilterEditorPopupWidget* N::FilterEditorWidget::popup() const
{
	return m_popup;
}

void N::FilterEditorWidget::setPopup(FilterEditorPopupWidget * widget)
{
  m_popup = widget ;
}

void N::FilterEditorWidget::showPopup(bool visible)
{
	if (m_popup){
		if (visible){
			m_popup->show();
			movePopup();
		} else {
			m_popup->hide();
		}
	}
}
