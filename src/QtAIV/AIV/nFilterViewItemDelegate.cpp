#include "nAivPrivate.hpp"

N::FilterViewItemDelegate:: FilterViewItemDelegate (QObject * parent)
                          : QStyledItemDelegate    (          parent)
{
  m_enableFilter = false ;
}

N::FilterViewItemDelegate::~FilterViewItemDelegate (void)
{
}

void N::FilterViewItemDelegate::comboxBoxItemActivated(int index)
{
  Q_UNUSED         ( index               ) ;
  emit commitData  ( (QWidget*) sender() ) ;
  emit closeEditor ( (QWidget*) sender() ) ;
}

void N::FilterViewItemDelegate::cancelAndClose(QAbstractItemDelegate::EndEditHint hint)
{
  QWidget * w = qobject_cast<QWidget *>(sender()) ;
  emit closeEditor ( w , hint )                   ;
}

void N::FilterViewItemDelegate::commitAndClose(QAbstractItemDelegate::EndEditHint hint)
{
  QWidget * w = qobject_cast<QWidget *>(sender()) ;
  emit commitData  ( w        )                   ;
  emit closeEditor ( w , hint )                   ;
}

QWidget * N::FilterViewItemDelegate::createEditor (
            QWidget                    * parent   ,
            const QStyleOptionViewItem & option   ,
            const QModelIndex          & index    ) const
{
  const AbstractFilterModel * model = qobject_cast<const AbstractFilterModel *>(index.model()) ;
  nIfSafe(model) {
    AbstractFilter * filter = model->filter(index) ;
    nIfSafe(filter) {
      return filter->createEditor((FilterViewItemDelegate *)this, parent, option, index) ;
    } else {
      FilterView * view = qobject_cast<FilterView *>(parent->parentWidget()) ;
      if (view) {
        QVariantMap properties;
        properties [ "column" ] = index.column();
        properties [ "row"    ] = index.row();
        properties [ "type"   ] = index.data(AbstractFilterModel::DefaultFilterTypeRole).toInt() ;
        filter = model->createFilter(index, properties) ;
        if (filter){
          filter->setEnabled(true);
          return filter->createEditor((FilterViewItemDelegate*)this, parent, option, index) ;
        }
      }
    }
  }
  return 0 ;
}

void N::FilterViewItemDelegate::lineEditReturnPressed(void)
{
  emit commitData  ( (QWidget *)sender() ) ;
  emit closeEditor ( (QWidget *)sender() ) ;
}

void N::FilterViewItemDelegate::listWidgetCurrentItemChanged (
       QListWidgetItem * current                             ,
       QListWidgetItem * previous                            )
{ Q_UNUSED         ( current            ) ;
  Q_UNUSED         ( previous           ) ;
  emit commitData  ( (QWidget*)sender() ) ;
  emit closeEditor ( (QWidget*)sender() ) ;
}

void N::FilterViewItemDelegate::setEditorData (
       QWidget           * editor             ,
       const QModelIndex & index              ) const
{
  const AbstractFilterModel * model = qobject_cast<const AbstractFilterModel *>(index.model()) ;
  if (NotNull(model))                              {
    AbstractFilter * filter = model->filter(index) ;
    if (NotNull(filter))                           {
      return filter->setEditorData(editor, index)  ;
    }                                              ;
  }                                                ;
}

void N::FilterViewItemDelegate::setModelData (
       QWidget            * editor           ,
       QAbstractItemModel * model            ,
       const QModelIndex  & index            ) const
{
  const AbstractFilterModel * m = qobject_cast<const AbstractFilterModel *>(index.model()) ;
  if (NotNull(m))                         {
    AbstractFilter * f = m->filter(index) ;
    if (NotNull(f))                       {
      f->setModelData(editor,model,index) ;
    }                                     ;
  }                                       ;
}

QSize N::FilterViewItemDelegate::sizeHint   (
        const QStyleOptionViewItem & option ,
        const QModelIndex          & index  ) const
{
  return QStyledItemDelegate :: sizeHint ( option , index ) ;
}

void N::FilterViewItemDelegate::updateEditorGeometry (
       QWidget                    * editor           ,
       const QStyleOptionViewItem & option           ,
       const QModelIndex          & index            ) const
{
  const AbstractFilterModel * m = qobject_cast<const AbstractFilterModel *>(index.model()) ;
  if (NotNull(m))                                  {
    AbstractFilter * f = m->filter(index)          ;
    if (NotNull(f))                                {
      f->updateEditorGeometry(editor,option,index) ;
    }                                              ;
  }                                                ;
}
