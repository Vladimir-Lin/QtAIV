#include <qtaiv.h>

N::FilterEditorPopupWidget:: FilterEditorPopupWidget ( QWidget * parent , Plan * p               )
                           : QFrame                  (           parent , Qt::Popup | Qt::Window )
                           , VirtualGui              (           this   ,        p               )
{
}

N::FilterEditorPopupWidget::~FilterEditorPopupWidget(void)
{
}

bool N::FilterEditorPopupWidget::cancelAndClose(QObject * obj,int key) const
{
  Q_UNUSED(obj) ;
  Q_UNUSED(key) ;
  return false  ;
}

bool N::FilterEditorPopupWidget::commitAndClose(QObject * obj,int key) const
{
  Q_UNUSED(obj) ;
  Q_UNUSED(key) ;
  return false  ;
}
