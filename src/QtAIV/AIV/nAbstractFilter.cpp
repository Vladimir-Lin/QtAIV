#include "nAivPrivate.hpp"

#define d ((AbstractFilterPrivate *)D)

N::AbstractFilter:: AbstractFilter ( void                                  )
                  : D              ((void *)new AbstractFilterPrivate(this))
{
//  cGroup = 0;
  d -> properties [ "enabled"        ] = false                ;
  d -> properties [ "type"           ] = AbstractFilter::Type ;
  d -> properties [ "column"         ] = -1                   ;
  d -> properties [ "highlightColor" ] = QColor("red")        ;
}

N::AbstractFilter:: AbstractFilter (int type,int row,int column            )
                  : D              ((void *)new AbstractFilterPrivate(this))
{
  d -> properties [ "enabled"        ] = false         ;
  d -> properties [ "type"           ] = type          ;
  d -> properties [ "row"            ] = row           ;
  d -> properties [ "column"         ] = column        ;
  d -> properties [ "highlightColor" ] = QColor("red") ;
}

N::AbstractFilter:: AbstractFilter (const QMap<QString,QVariant> & properties)
                  : D              ((void *)new AbstractFilterPrivate(this)  )
{
  QMapIterator<QString,QVariant> mIt(properties) ;
  while (mIt.hasNext())                          {
    mIt . next ( )                               ;
    d  -> properties [ mIt.key() ] = mIt.value() ;
  }                                              ;
}

N::AbstractFilter::~AbstractFilter(void)
{
  delete d ;
}

int N::AbstractFilter::column(void) const
{
  return d -> properties . value ( "column" , -1 ) . toInt ( ) ;
}

void N::AbstractFilter::addContextMenuActions(QMenu * menu,QWidget * receiver)
{ Q_UNUSED ( menu     ) ;
  Q_UNUSED ( receiver ) ;
}

QColor N::AbstractFilter::highlightColor(void) const
{
  return qvariant_cast<QColor>(property("highlightColor")) ;
}

bool N::AbstractFilter::isEnabled(void) const
{
  if (!isValid()) return false                                      ;
  return d -> properties . value ( "enabled" , false ) . toBool ( ) ;
}

bool N::AbstractFilter::isValid(void) const
{
  return d -> properties . value ( "type"   ) . toInt ( ) != N::AbstractFilter::Type &&
         d -> properties . value ( "column" ) . toInt ( )  > -1                       ;
}

QMap<QString,QVariant> N::AbstractFilter::properties(void) const
{
  return d -> properties ;
}

int N::AbstractFilter::row(void) const
{
  return d -> properties . value ( "row" , -1 ) . toInt ( ) ;
}

QVariant N::AbstractFilter::property(const QString & key,const QVariant & defaultValue) const
{
  return d -> properties . value ( key , defaultValue ) ;
}

void N::AbstractFilter::setEnabled(bool on)
{
  d -> properties [ "enabled" ] = on ;
}

void N::AbstractFilter::setHighlightColor(const QColor & color)
{
  setProperty ( "highlightColor" , color ) ;
}

void N::AbstractFilter::setProperty(const QString & name,const QVariant & value)
{
  d -> properties [ name ] = value ;
}

void N::AbstractFilter::setRow(int row)
{
  d -> properties [ "row" ] = row ;
}

int N::AbstractFilter::type(void) const
{
  return d -> properties . value ( "type" , N::AbstractFilter::Type ) . toInt ( ) ;
}
