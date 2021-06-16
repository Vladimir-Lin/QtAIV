#include "nAivPrivate.hpp"

#define d ((AbstractFilterModelPrivate *)D)

N::AbstractFilterModel:: AbstractFilterModel (QObject * parent)
                       : QAbstractItemModel  (          parent)
                       , D ( (void *)new AbstractFilterModelPrivate(this) )
{
  d -> filterGroupList.append(new FilterGroup(tr("<default>"))) ;
  d -> matchMode   = AdvancedItemViews::MatchNormal             ;
  d -> mode        = AdvancedItemViews::FilterMode              ;
  d -> sourceModel = 0                                          ;
}

N::AbstractFilterModel::~AbstractFilterModel(void)
{
  delete d ;
  D = NULL ;
}

int N::AbstractFilterModel::columnCount(const QModelIndex & parent) const
{
  nIfSafe(d->sourceModel)                      {
    return d->sourceModel->columnCount(parent) ;
  }                                            ;
  return 1                                     ;
}

QVariant N::AbstractFilterModel::data(const QModelIndex & index, int role) const
{
  if (!index.isValid()) return QVariant() ;
  AbstractFilter * filter = d->filterGroupList.at(index.row())->filterAtColumn(index.column());
  if (role == Qt::DecorationRole) {
    if (filter){
      if (filter->isEnabled()){
        return QIcon(":/qaiv/filter/enabled");
      } else {
        return QIcon(":/qaiv/filter/disabled");
      }
    }
    return QVariant();
  } else
  if (role == N::AbstractFilterModel::DefaultFilterTypeRole) {
    return d->defaultFilterType.value(index.column(),AbstractFilter::Type) ;
  } else
  if (role == Qt::DisplayRole) {
    if (filter){
      return filter->data();
    } else {
      return tr("<all>");
    }
  } else
  if (role == Qt::EditRole) {
    filter = d->filterGroupList.at(index.row())->filterAtColumn(index.column()) ;
    if (filter){
      return filter->properties();
    } else {
      return QVariant();
    }
  } else
  if (role == N::AbstractFilterModel::ColumnFilterTypesRole) {
    return d->columnFilterTypes.value(index.column());
  } else
  if (role == N::AbstractFilterModel::ValueFilterTypeRole){
    return d->valueTypeMap.value(index.column(), -1) ;
  }
  return QVariant();
}

N::AbstractFilter * N::AbstractFilterModel::filter(const QModelIndex & index) const
{
  if (index.isValid() && index.model() == this) {
    return d->filterGroupList.at(index.row())->filterAtColumn(index.column());
  }
  return 0;
}

N::FilterGroup * N::AbstractFilterModel::filterGroup(const QModelIndex & index) const
{
  if (index.row() < d->filterGroupList.size()) {
    return d->filterGroupList.at(index.row())  ;
  }                                            ;
  return 0                                     ;
}

QList<N::AbstractFilter *> N::AbstractFilterModel::filtersAtRow(int row) const
{
  if (row < d->filterGroupList.size())           {
    return d->filterGroupList.at(row)->filters() ;
  }                                              ;
  return QList<AbstractFilter *>()               ;
}

Qt::ItemFlags N::AbstractFilterModel::flags(const QModelIndex & index) const
{ Q_UNUSED ( index )        ;
  Qt::ItemFlags f           ;
  f |= Qt::ItemIsEditable   ;
  f |= Qt::ItemIsEnabled    ;
  f |= Qt::ItemIsSelectable ;
  return f                  ;
}

N::FilterGroup * N::AbstractFilterModel::groupAt(int row) const
{
  if (row < d->filterGroupList.size()) {
    return d->filterGroupList.at(row)  ;
  }                                    ;
  return 0                             ;
}

QVariant N::AbstractFilterModel::headerData (
           int             section          ,
           Qt::Orientation orientation      ,
           int             role             ) const
{
  if (orientation == Qt::Vertical)                  {
    if (role == Qt::DisplayRole)                    {
      return d->filterGroupList.at(section)->name() ;
    }
  } else
  if (orientation == Qt::Horizontal) {
    if (d->sourceModel){
      if (role == Qt::DisplayRole){
        return d->sourceModel->headerData(section, Qt::Horizontal, Qt::DisplayRole);
      }
    }
  }
  return QAbstractItemModel::headerData(section, orientation, role);
}

QModelIndex N::AbstractFilterModel::index(int row,int column,const QModelIndex & parent) const
{ Q_UNUSED           ( parent       ) ;
  return createIndex ( row , column ) ;
}

bool N::AbstractFilterModel::insertRows(int row,int count,const QModelIndex & parent)
{ Q_UNUSED(parent);
  if (count == 0) return false ;
  beginInsertRows(parent, row, row + count - 1);
  if (row == rowCount()){
    for (int iCount = 0; iCount < count; iCount++){
      d->filterGroupList.append(new FilterGroup(tr("<filter>")));
    }
  } else {
    for (int iRow = row; iRow < d->filterGroupList.size(); iRow++){
      for (int iFilter = 0; iFilter < d->filterGroupList.at(iRow)->count(); iFilter++){
        d->filterGroupList.at(iRow)->filter(iFilter)->setRow(d->filterGroupList.at(iRow)->filter(iFilter)->row() + count);
      }
    }
    for (int iCount = 0; iCount < count; iCount++){
      d->filterGroupList.insert(row,new FilterGroup(tr("<filter>")));
    }
  }
  endInsertRows();
  return true;
}

N::AdvancedItemViews::FilterMatchMode N::AbstractFilterModel::matchMode(void) const
{
  return d -> matchMode ;
}

N::AdvancedItemViews::FilterProxyMode N::AbstractFilterModel::mode(void) const
{
  return d -> mode ;
}

QModelIndex N::AbstractFilterModel::parent(const QModelIndex & index) const
{ Q_UNUSED           ( index ) ;
  return QModelIndex (       ) ;
}

QList<N::AbstractFilterModel::FilterTypeEntry> N::AbstractFilterModel::registeredFilterTypes(void) const
{
  return d->filterTypes ;
}

bool N::AbstractFilterModel::removeRows(int row,int count,const QModelIndex & parent)
{ Q_UNUSED(parent);
  if (count == 0) return false ;
  beginRemoveRows(QModelIndex(), row, row + count - 1);
  for (int iCount = 0; iCount < count; iCount++){
    d->filterGroupList.removeAt(row);
  }
  for (int iRow = row; iRow < d->filterGroupList.size(); iRow++){
    for (int iFilter = 0; iFilter < d->filterGroupList.at(iRow)->count(); iFilter++){
      d->filterGroupList.at(iRow)->filter(iFilter)->setRow(d->filterGroupList.at(iRow)->filter(iFilter)->row() - count);
    }
  }
  endRemoveRows() ;
  return true ;
}

int N::AbstractFilterModel::rowCount(const QModelIndex & parent) const
{ Q_UNUSED ( parent )              ;
  return d->filterGroupList.size() ;
}

bool N::AbstractFilterModel::setData (
       const QModelIndex & index     ,
       const QVariant    & value     ,
       int                 role      )
{
  if (!index.isValid()) return false ;
  if (role == N::AbstractFilterModel::DefaultFilterTypeRole){
    d->defaultFilterType[index.column()] = value.toInt();
  } else
  if (role == Qt::EditRole) {
    if (value.toMap().isEmpty()){
      d->filterGroupList.at(index.row())->remove(index.column());
    } else {
      AbstractFilter * filter = d->filterGroupList.at(index.row())->filterAtColumn(index.column()) ;
      if (filter == 0) {
        // Create new filter
        filter = createFilter(index, value.toMap());
        if (filter == 0) return false ;
      } else {
        QMapIterator<QString,QVariant> it(value.toMap());
        while (it.hasNext()) {
          it.next();
          filter->setProperty(it.key(), it.value());
        }
      }
    }
    emit dataChanged(index, index);
    return true;
  } else
  if (role == N::AbstractFilterModel::ColumnFilterTypesRole) {
    d->columnFilterTypes[index.column()] = value.toList() ;
  } else
  if (role == N::AbstractFilterModel::ValueFilterTypeRole) {
    d->valueTypeMap[index.column()] = value.toInt() ;
  }
  return false;
}

void N::AbstractFilterModel::setDefaultFilterType(int column,int type)
{
  d -> defaultFilterType [ column ] = type ;
}

bool N::AbstractFilterModel::setHeaderData (
       int              section            ,
       Qt::Orientation  orientation        ,
       const QVariant & value              ,
       int              role               )
{
  if ( role != Qt::EditRole ) return false                    ;
  if (orientation == Qt::Vertical)                            {
    d->filterGroupList.at(section)->setName(value.toString()) ;
    emit headerDataChanged(Qt::Horizontal, section, section)  ;
    return true                                               ;
  }                                                           ;
  return false                                                ;
}

void N::AbstractFilterModel::setMatchMode(AdvancedItemViews::FilterMatchMode flag)
{
  if (d->matchMode != flag)             {
    d->matchMode = flag                 ;
    emit matchModeChanged(d->matchMode) ;
  }
}

void N::AbstractFilterModel::setMode(AdvancedItemViews::FilterProxyMode mode)
{
  if (d->mode != mode)        {
    d->mode = mode            ;
    emit modeChanged(d->mode) ;
  }
}

void N::AbstractFilterModel::setSourceModel(QAbstractItemModel * model)
{
  nDropOut ( IsNull(model) )                                          ;
  d->sourceModel = model                                              ;
  connect(d->sourceModel , SIGNAL(layoutChanged           ())         ,
          this           , SLOT  (sourceModelLayoutChanged())       ) ;
  headerDataChanged(Qt::Horizontal,0,d->sourceModel->columnCount()-1) ;
}

QAbstractItemModel * N::AbstractFilterModel::sourceModel(void) const
{
  return d -> sourceModel ;
}

void N::AbstractFilterModel::sourceModelLayoutChanged(void)
{
  nIfSafe(d->sourceModel)                                               {
    headerDataChanged(Qt::Horizontal,0,d->sourceModel->columnCount()-1) ;
  }                                                                     ;
}

bool N::AbstractFilterModel::registerFilter (
       int             type                 ,
       const QString & text                 ,
       const QString & toolTip              )
{
  return registerFilter ( type , QIcon() , text , toolTip ) ;
}

bool N::AbstractFilterModel::registerFilter (
       int             type                 ,
       const QIcon   & icon                 ,
       const QString & text                 ,
       const QString & toolTip              )
{
  Q_FOREACH(FilterTypeEntry e,d->filterTypes) {
    if (e.type == type) return false          ;
  }                                           ;
  /////////////////////////////////////////////
  FilterTypeEntry e                           ;
  e  . type         = type                    ;
  e  . icon         = icon                    ;
  e  . text         = text                    ;
  e  . tooltip      = toolTip                 ;
  d -> filterTypes << e                       ;
  return true                                 ;
}
