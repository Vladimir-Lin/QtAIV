#include "nAivPrivate.hpp"

#define MAGICNUMBER 0x544156
#define d ((ConditionalDecorationProxyModelPrivate *)D)

N::ConditionalDecorationProxyModel:: ConditionalDecorationProxyModel (QObject * parent)
                                   : QSortFilterProxyModel           (          parent)
                                   , D ( (void *)new ConditionalDecorationProxyModelPrivate(this) )
{
  QVariantMap m ;

  m["blue"  ] = ":/qaiv/flag/blue"   ;
  m["green" ] = ":/qaiv/flag/green"  ;
  m["orange"] = ":/qaiv/flag/orange" ;
  m["pink"  ] = ":/qaiv/flag/pink"   ;
  m["purple"] = ":/qaiv/flag/purple" ;
  m["red"   ] = ":/qaiv/flag/red"    ;
  m["yellow"] = ":/qaiv/flag/yellow" ;
  d->iconSets["flags"] = m;

  m.clear();
  m["red (on)"   ] = ":/qaiv/led/red.on"    ;
  m["orange (on)"] = ":/qaiv/led/amber.on"  ;
  m["blue (on)"  ] = ":/qaiv/led/blue.on"   ;
  m["green (on)" ] = ":/qaiv/led/green.on"  ;
  m["white (on)" ] = ":/qaiv/led/white.on"  ;
  m["yellow (on)"] = ":/qaiv/led/yellow.on" ;
  d->iconSets["leds"] = m;

  m.clear();
  m["Blue"  ] = ":/qaiv/tag/blue"   ;
  m["Green" ] = ":/qaiv/tag/green"  ;
  m["Orange"] = ":/qaiv/tag/orange" ;
  m["Pink"  ] = ":/qaiv/tag/pink"   ;
  m["Purple"] = ":/qaiv/tag/purple" ;
  m["Red"   ] = ":/qaiv/tag/red"    ;
  m["Yellow"] = ":/qaiv/tag/yellow" ;
  d->iconSets["tags"] = m;

  m.clear();
  m["Happy"  ] = ":/qaiv/emoticon/smiley.happy"   ;
  m["Neutral"] = ":/qaiv/emoticon/smiley.neutral" ;
  m["Unhappy"] = ":/qaiv/emoticon/smiley.unhappy" ;
  d->iconSets["emoticon"] = m;

}

N::ConditionalDecorationProxyModel::~ConditionalDecorationProxyModel(void)
{
  delete d ;
  D = NULL ;
}

void N::ConditionalDecorationProxyModel::addDecoration(int column,AbstractItemModelDecoration * decoration)
{
  decoration->setProperty("column", column)   ;
  d->columnDecorationMap[column] = decoration ;
  invalidate()                                ;
}

void N::ConditionalDecorationProxyModel::addIcon(const QString & set,const QString & name,const QString & resource)
{
  QVariantMap m = d->iconSets.value(set).toMap();
  m[name] = resource;
  d->iconSets[set] = m;
}

void N::ConditionalDecorationProxyModel::addIconSet(const QString & name,const QVariantMap icons)
{
  d->iconSets[name] = icons ;
}

QVariant N::ConditionalDecorationProxyModel::data(const QModelIndex &index,int role) const
{
  if (!index.isValid()) return QVariant() ;
  if (role == Qt::DecorationRole) {
    AbstractItemModelDecoration * decoration = d->columnDecorationMap.value(index.column(), 0);
    if (decoration){
      return decoration->decorate(index);
    }
  } else
  if (role == N::ConditionalDecorationProxyModel::ConditionalDecorationRole) {
    AbstractItemModelDecoration * decoration = d->columnDecorationMap.value(index.column(), 0) ;
    if (decoration) return decoration->properties() ;
    return QVariant() ;
  } else
  if (role == N::ConditionalDecorationProxyModel::IconSetsRole) {
    return d->iconSets ;
  }
  return QSortFilterProxyModel::data(index,role) ;
}

QIcon N::ConditionalDecorationProxyModel::icon(const QString &set,const QString &name) const
{
  QVariantMap m = d->iconSets.value(set).toMap();
  if (!m.isEmpty()){
    QVariant i = m.value(name);
    if (i.type() == QVariant::Icon){
      return qvariant_cast<QIcon>(i);
     }
     return QIcon(i.toString());
  }
  qWarning() << "icon set" << set << "not found";
  return QIcon() ;
}

WMAPs N::ConditionalDecorationProxyModel::iconSets(void) const
{
  return d->iconSets ;
}

QSize N::ConditionalDecorationProxyModel::iconSize(void) const
{
  return d->iconSize ;
}

int N::ConditionalDecorationProxyModel::iconSpacing(void) const
{
  return d->iconSpacing ;
}

bool N::ConditionalDecorationProxyModel::restoreState(const QByteArray & state)
{
  QDataStream s(state) ;
  int     count        ;
  quint32 mn           ;
  quint32 v            ;
  s >> mn              ;
  if (mn != MAGICNUMBER) {
    qWarning() << Q_FUNC_INFO << "magic number invalid" ;
    return false;
  }
  s >> v ;
  if (v =! 1){
    qWarning() << Q_FUNC_INFO
               << "version"
               << v
               << "not supported" ;
    return false                  ;
  }
  s >> d->iconSize
    >> d->iconSpacing
    >> count          ;
  for (int i = 0; i < count; i++){
    QVariantMap p;
    s >> p;
    ConditionalDecoration * deco = new ConditionalDecoration() ;
    deco->setProperties(p) ;
  }
  return true ;
}

QByteArray N::ConditionalDecorationProxyModel::stateState(void) const
{
  QByteArray    ba  ;
  QDataStream s(ba) ;

  s << (quint32)MAGICNUMBER
    << (quint32)1
    << d->iconSize
    << d->iconSpacing
    << d->columnDecorationMap.size() ;

  QMapIterator<int,AbstractItemModelDecoration*> it(d->columnDecorationMap) ;
  while ( it.hasNext() )          {
    s << it.value()->properties() ;
    it.next()                     ;
  }
  return ba ;
}

bool N::ConditionalDecorationProxyModel::setData (
       const QModelIndex & index                 ,
       const QVariant    & value                 ,
       int                 role                  )
{
  if (role == N::ConditionalDecorationProxyModel::ConditionalDecorationRole) {
    AbstractItemModelDecoration * decoration = d->columnDecorationMap.value(index.column(), 0);
    if (decoration){
      decoration->setProperties(value.toMap());
    } else {
      decoration = new ConditionalDecoration(index.column()) ;
      decoration->setProperties(value.toMap()) ;
      d->columnDecorationMap[index.column()] = decoration ;
    }
    return true;
  }
  return QSortFilterProxyModel::setData(index,value,role) ;
}

void N::ConditionalDecorationProxyModel::setIconSize(const QSize & size)
{
  if (size != d->iconSize) {
    d->iconSize = size     ;
    invalidateFilter()     ;
  }                        ;
}

void N::ConditionalDecorationProxyModel::setIconSpacing(int spacing)
{
  if (spacing != d->iconSpacing) {
    d->iconSpacing = spacing     ;
    invalidateFilter ( )         ;
  }                              ;
}
