#ifndef QAIVPRIVATE_HPP
#define QAIVPRIVATE_HPP

#include <QtAIV>

namespace N
{

class AbstractFilterPrivate                  ;
class AbstractFilterModelPrivate             ;
class AbstractFilterProxyModelPrivate        ;
class AbstractItemModelDecorationPrivate     ;
class ConditionalDecorationProxyModelPrivate ;
class CheckStateProxyModelPrivate            ;
class GroupingProxyModelGroup                ;
class GroupingProxyModelPrivate              ;
class SharedItemSelectionModelPrivate        ;
class SingleColumnProxyModelPrivate          ;
class UniqueValuesProxyModelPrivate          ;
class FixedRowsFilterProxyModelPrivate       ;
class FixedRowsDecorationProxyModelPrivate   ;
class FixedRowsTableViewPrivate              ;
class FilterViewPrivate                      ;

class AbstractFilterPrivate
{
  public:

    QIcon            icon       ;
    FilterGroup    * group      ;
    WMAPs            properties ;
    AbstractFilter * f          ;

    explicit AbstractFilterPrivate (AbstractFilter * af) ;
    virtual ~AbstractFilterPrivate (void) ;

};

class AbstractFilterModelPrivate
{
  public:

    IMAPs                                       defaultFilterType ;
    QMap<int,QVariantList>                      columnFilterTypes ;
    QList<FilterGroup *>                        filterGroupList   ;
    AdvancedItemViews::FilterProxyMode          mode              ;
    AdvancedItemViews::FilterMatchMode          matchMode         ;
    QAbstractItemModel                        * sourceModel       ;
    IMAPs                                       valueTypeMap      ;
    QList<AbstractFilterModel::FilterTypeEntry> filterTypes       ;
    AbstractFilterModel                       * m                 ;

    explicit AbstractFilterModelPrivate (AbstractFilterModel * fm) ;
    virtual ~AbstractFilterModelPrivate (void) ;

};

class AbstractFilterProxyModelPrivate
{
  public:

    AbstractFilterModel      * filterModel     ;
    int                        lastResultCount ;
    AbstractFilterProxyModel * m               ;

    explicit AbstractFilterProxyModelPrivate (AbstractFilterProxyModel * pm) ;
    virtual ~AbstractFilterProxyModelPrivate (void) ;

};

class AbstractItemModelDecorationPrivate
{
  public:

    WMAPs                         properties ;
    AbstractItemModelDecoration * m          ;

    explicit AbstractItemModelDecorationPrivate (AbstractItemModelDecoration * md) ;
    virtual ~AbstractItemModelDecorationPrivate (void) ;

};

class ConditionalDecorationProxyModelPrivate
{
  public:

   QMap<int,AbstractItemModelDecoration *> columnDecorationMap ;
   WMAPs                                   iconSets            ;
   QSize                                   iconSize            ;
   int                                     iconSpacing         ;
   ConditionalDecorationProxyModel      *  m                   ;

    explicit ConditionalDecorationProxyModelPrivate  (ConditionalDecorationProxyModel * pm) ;
    virtual ~ConditionalDecorationProxyModelPrivate (void) ;

};

class CheckStateProxyModelPrivate
{
  public:

    QList<QModelIndex>     checkedIndexes ;
    CUIDs                  columns        ;
    CheckStateProxyModel * m              ;

    explicit CheckStateProxyModelPrivate (CheckStateProxyModel * pm) ;
    virtual ~CheckStateProxyModelPrivate (void);

};

class GroupingProxyModelGroup
{
  public:

    explicit GroupingProxyModelGroup (GroupingProxyModelGroup * parent = 0) ;
    explicit GroupingProxyModelGroup (int sourceModelRow,GroupingProxyModelGroup * parent = 0) ;
    explicit GroupingProxyModelGroup (const QString & name,GroupingProxyModelGroup * parent = 0) ;
    virtual ~GroupingProxyModelGroup (void) ;

    void addSourceModelRow(int row) ;
    void clear(void) ;
    GroupingProxyModelGroup * child(int index) const ;
    int childCount(void) const ;
    QVariant data(int role) const ;
    GroupingProxyModelGroup * findSourceModelRow(int sourceModelRow) const ;
    GroupingProxyModelGroup * group(int sourceModelRow) const ;
    int indexOf(GroupingProxyModelGroup * group) const ;
    int sourceModelRowIndexAt(int sourceModelRow) const ;
    GroupingProxyModelGroup * matches(const QVariant & value) const ;
    void moveSourceRow(int count);
    GroupingProxyModelGroup * parent(void) const;
    void removeChild(int index) ;
    void removeChildAtSourceModelRow (int sourceModelRow) ;
    void removeSourceModelRow (int row) ;
    int row(void) const ;
    int row(int sourceModelRow) const ;
    void setData(const QVariant & data,int role) ;
    int sourceModelRow(void) const ;
    CUIDs sourceModelRows(void) const ;

  protected:

    QList<GroupingProxyModelGroup *> cChildren        ;
    QMap<int,QVariant>               cData            ;
    GroupingProxyModelGroup       *  cParent          ;
    int                              cSourceModelRow  ;
    CUIDs                            cSourceModelRows ;

  private:

};

class GroupingProxyModelPrivate
{
  public:

    int                       modelColumn       ;
    int                       groupItemDataRole ;
    GroupingProxyModelGroup * root              ;
    QAbstractItemModel      * sourceModel       ;
    GroupingProxyModel      * m                 ;

    explicit GroupingProxyModelPrivate (GroupingProxyModel * pm) ;
    virtual ~GroupingProxyModelPrivate (void) ;

};

class SharedItemSelectionModelPrivate
{
  public:

    QItemSelectionModel      * shared ;
    SharedItemSelectionModel * m      ;

    explicit SharedItemSelectionModelPrivate (SharedItemSelectionModel * sm) ;
    virtual ~SharedItemSelectionModelPrivate (void) ;

};

class SingleColumnProxyModelPrivate
{
  public:

    int                      modelColumn ;
    SingleColumnProxyModel * m           ;

    explicit SingleColumnProxyModelPrivate (SingleColumnProxyModel * pm) ;
    virtual ~SingleColumnProxyModelPrivate (void) ;

};

class UniqueValuesProxyModelPrivate
{
  public:

    bool                      emptyValues ;
    int                       modelColumn ;
    QMap<QString,QList<int> > valueMap    ;
    UniqueValuesProxyModel  * m           ;

    explicit UniqueValuesProxyModelPrivate(UniqueValuesProxyModel * pm) ;
    virtual ~UniqueValuesProxyModelPrivate(void) ;

};

class FixedRowsFilterProxyModelPrivate
{
  public:

    bool                         enabled ;
    QList<QPersistentModelIndex> rows    ;
    FixedRowsFilterProxyModel  * m       ;

    explicit FixedRowsFilterProxyModelPrivate (FixedRowsFilterProxyModel * pm) ;
    virtual ~FixedRowsFilterProxyModelPrivate (void);

};

class FixedRowsDecorationProxyModelPrivate
{
  public:

    bool                                enabled     ;
    QPointer<FixedRowsFilterProxyModel> filterProxy ;
    QList<QPersistentModelIndex>        rows        ;
    FixedRowsDecorationProxyModel     * m           ;

    explicit FixedRowsDecorationProxyModelPrivate (FixedRowsDecorationProxyModel * pm) ;
    virtual ~FixedRowsDecorationProxyModelPrivate (void) ;

};

class FixedRowsTableViewPrivate
{
  public:

    FixedRowsFilterProxyModel     * filterProxy     ;
    FixedRowsDecorationProxyModel * decorationProxy ;
    FixedRowsTableView            * v               ;

    explicit FixedRowsTableViewPrivate (FixedRowsTableView * tv) ;
    virtual ~FixedRowsTableViewPrivate (void) ;

};

class FilterViewPrivate
{
  public:

    FilterViewConnector * filterViewConnector ;
    int                   allowedFilterTypes  ;
    int                   defaultFilterType   ;
    bool                  filterVisible       ;
    int                   maxFilterVisible    ;
    FilterView          * v                   ;

    explicit FilterViewPrivate (FilterView * fw) ;
    virtual ~FilterViewPrivate (void);

};

}

#endif // QAIVPRIVATE_HPP
