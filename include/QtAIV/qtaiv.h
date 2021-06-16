/****************************************************************************
 *                                                                          *
 * Copyright (C) 2015 Neutrino International Inc.                           *
 *                                                                          *
 * Author : Brian Lin <lin.foxman@gmail.com>, Skype: wolfram_lin            *
 *                                                                          *
 ****************************************************************************/

#ifndef QT_AIV_H
#define QT_AIV_H

#include <QtComponents>

QT_BEGIN_NAMESPACE

#ifndef QT_STATIC
#    if defined(QT_BUILD_QTAIV_LIB)
#      define Q_AIV_EXPORT Q_DECL_EXPORT
#    else
#      define Q_AIV_EXPORT Q_DECL_IMPORT
#    endif
#else
#    define Q_AIV_EXPORT
#endif


namespace N
{

class Q_AIV_EXPORT FilterEditorPopupWidget         ;
class Q_AIV_EXPORT FilterEditorWidget              ;
class Q_AIV_EXPORT FixedRowsHeaderView             ;
class Q_AIV_EXPORT HeaderViewConnector             ;
class Q_AIV_EXPORT AdvancedItemViews               ;
class Q_AIV_EXPORT FilterViewItemDelegate          ;
class Q_AIV_EXPORT FilterGroup                     ;
class Q_AIV_EXPORT AbstractFilterProxyModel        ;
class Q_AIV_EXPORT ConditionalDecorationProxyModel ;
class Q_AIV_EXPORT CheckStateProxyModel            ;
class Q_AIV_EXPORT GroupingProxyModel              ;
class Q_AIV_EXPORT SharedItemSelectionModel        ;
class Q_AIV_EXPORT SingleColumnProxyModel          ;
class Q_AIV_EXPORT UniqueValuesProxyModel          ;
class Q_AIV_EXPORT FixedRowsFilterProxyModel       ;
class Q_AIV_EXPORT FixedRowsDecorationProxyModel   ;
class Q_AIV_EXPORT AbstractItemModelDecoration     ;
class Q_AIV_EXPORT AbstractFilterModel             ;
class Q_AIV_EXPORT AbstractFilter                  ;
class Q_AIV_EXPORT ConditionalDecoration           ;
class Q_AIV_EXPORT AutoFilterEditorPopup           ;
class Q_AIV_EXPORT AutoFilterEditor                ;
class Q_AIV_EXPORT AutoFilter                      ;
class Q_AIV_EXPORT RangeFilterEditorPopup          ;
class Q_AIV_EXPORT RangeFilterEditor               ;
class Q_AIV_EXPORT RangeFilter                     ;
class Q_AIV_EXPORT RegExpFilterEditor              ;
class Q_AIV_EXPORT RegExpFilter                    ;
class Q_AIV_EXPORT SelectionListFilterEditorPopup  ;
class Q_AIV_EXPORT SelectionListFilterEditor       ;
class Q_AIV_EXPORT SelectionListFilter             ;
class Q_AIV_EXPORT SpecialFilterEditor             ;
class Q_AIV_EXPORT SpecialFilter                   ;
class Q_AIV_EXPORT TextFilterEditor                ;
class Q_AIV_EXPORT TextFilter                      ;
class Q_AIV_EXPORT ValueFilterEditor               ;
class Q_AIV_EXPORT ValueFilter                     ;
class Q_AIV_EXPORT FilterModel                     ;
class Q_AIV_EXPORT AdvancedHeaderView              ;
class Q_AIV_EXPORT FixedRowsTableView              ;
class Q_AIV_EXPORT FilterViewConnector             ;
class Q_AIV_EXPORT FilterView                      ;

/*****************************************************************************
 *                                                                           *
 *                           Enhanced Item View                              *
 *                                                                           *
 *****************************************************************************/

class Q_AIV_EXPORT FilterEditorPopupWidget : public QFrame
                              , public VirtualGui
{
  Q_OBJECT
  public:

    explicit FilterEditorPopupWidget (StandardConstructor) ;
    virtual ~FilterEditorPopupWidget (void) ;

    virtual bool cancelAndClose      (QObject* obj,int key) const ;
    virtual bool commitAndClose      (QObject* obj,int key) const ;

  protected:

  private:

  public slots:

  protected slots:

  private slots:

  signals:

};

class Q_AIV_EXPORT FilterEditorWidget : public Widget
{
  Q_OBJECT
  public:

    explicit FilterEditorWidget (StandardConstructor) ;
    virtual ~FilterEditorWidget (void) ;

    FilterEditorPopupWidget * popup (void) const ;
    void setPopup (FilterEditorPopupWidget * widget) ;

  protected:

    virtual bool eventFilter (QObject* object,QEvent *event);
    virtual void moveEvent   (QMoveEvent* e);

  private:

    void movePopup (void) ;

    FilterEditorPopupWidget * m_popup ;

  public slots:

    void showPopup(bool visible = true) ;

  protected slots:

  private slots:

  signals:

    void commit(void) ;
    void commitAndClose(QAbstractItemDelegate::EndEditHint hint = QAbstractItemDelegate::NoHint) ;
    void cancelAndClose(QAbstractItemDelegate::EndEditHint hint = QAbstractItemDelegate::NoHint) ;

};

class Q_AIV_EXPORT FixedRowsHeaderView : public HeaderView
{
  Q_OBJECT
  public:

    explicit FixedRowsHeaderView (Qt::Orientation orientation,QWidget * parent = NULL,Plan * plan = NULL) ;
    virtual ~FixedRowsHeaderView (void) ;

  protected:

    virtual void mouseReleaseEvent (QMouseEvent* event);

  private:

  public slots:

  protected slots:

  private slots:

  signals:

};

class Q_AIV_EXPORT HeaderViewConnector : public QObject
{
  Q_OBJECT
  public:

    explicit HeaderViewConnector (Qt::Orientation orientation, QObject * parent = 0) ;
    virtual ~HeaderViewConnector (void) ;

    void append(QHeaderView* view);

  protected:

    bool                           cAdjustSectionSizePending ;
    QList<QPointer< QHeaderView> > cHeaderViewList           ;
    Qt::Orientation                cOrientation              ;

  private:

  public slots:

    void adjustSectionSize();

  protected slots:

    void scrollBarRangeChanged(int min, int max);
    void scrollBarSilderMoved(int value);
    void sectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex);
    void sectionResized(int logicalIndex, int oldSize, int newSize);
    void sortIndicatorChanged(int logicalIndex, Qt::SortOrder order);
    void viewDestroyed(QObject* object);

  private slots:

  signals:

};

class Q_AIV_EXPORT AdvancedItemViews
{
  public:

    explicit AdvancedItemViews(void) ;
    virtual ~AdvancedItemViews(void) ;

    enum FilterProxyMode {
      FilterMode         ,
      HighlightMode    } ;

    enum FilterMatchMode {
      MatchNormal        , /*<! The filter matches if the condition is fullfilled */
      MatchInverted    } ; /*<! The filter matches if the condition is not fullfilled */

    enum ItemDataRole                                    {
      AutoFilterRole              = Qt::UserRole + 42101 ,
      ValueFilterTypeRole                                ,
      DefaultFilterTypeRole                              ,
      ColumnFilterTypesRole                              ,
      ConditionalDecorationRole                          ,
      SelectionListFilterDataRole                        ,
      SelectionFilterRole                                ,
      ValueRole                                          ,
      IconSetsRole                                     } ;

};

QModelIndex                qSourceIndex (const QModelIndex  & index) ;
const QAbstractItemModel * qSourceModel (const QModelIndex  & index) ;
QAbstractItemModel       * qSourceModel (QAbstractItemModel * model) ;

template <class T>
inline T qsourcemodel_cast(const QModelIndex & index)
{
  QModelIndex i(index)             ;
  T s = qobject_cast<T>(i.model()) ;
  if (s) return s                  ;
  QAbstractProxyModel * p          ;
  while ((p = qobject_cast<QAbstractProxyModel*>((QAbstractProxyModel*)i.model())))
  {
    T s = qobject_cast<T>(p)       ;
    if (s) return s                ;
    i = p->mapToSource(i)          ;
  }                                ;
  return 0                         ;
}

template <class T> inline T qsourcemodel_cast(QAbstractItemModel * model)
{
  QAbstractItemModel * m = model                      ;
  T s = qobject_cast<T>(m)                            ;
  if (s) return s                                     ;
  QAbstractProxyModel * p                             ;
  while ((p = qobject_cast<QAbstractProxyModel*>(m))) {
    T s = qobject_cast<T>(p->sourceModel())           ;
    if (s) return s                                   ;
    m = p->sourceModel()                              ;
  }                                                   ;
  return 0                                            ;
}

template <class T>
inline T qsourcemodel_cast(const QAbstractItemModel* model)
{
  QAbstractItemModel * m = (QAbstractItemModel*)model ;
  T s = qobject_cast<T>(m)                            ;
  if (s) return s                                     ;
  QAbstractProxyModel * p                             ;
  while((p = qobject_cast<QAbstractProxyModel*>(m)))  {
    T s = qobject_cast<T>(p->sourceModel())           ;
    if (s) return s                                   ;
    m = p->sourceModel()                              ;
  }                                                   ;
  return 0                                            ;
}

class Q_AIV_EXPORT FilterViewItemDelegate : public QStyledItemDelegate
{
  Q_OBJECT
  public:

    explicit FilterViewItemDelegate        (QObject * parent) ;
    virtual ~FilterViewItemDelegate        (void) ;

    QWidget * createEditor                 (QWidget                    * parent ,
                                            const QStyleOptionViewItem & option ,
                                            const QModelIndex          & index  ) const;
    void      setEditorData                (QWidget                    * editor ,
                                            const QModelIndex          & index  ) const;
    void      setModelData                 (QWidget                    * editor ,
                                            QAbstractItemModel         * model  ,
                                            const QModelIndex          & index  ) const;
    QSize     sizeHint                     (const QStyleOptionViewItem & option ,
                                            const QModelIndex          & index  ) const;
    void      updateEditorGeometry         (QWidget                    * editor ,
                                            const QStyleOptionViewItem & option ,
                                            const QModelIndex          & index  ) const;

  private:

    bool m_enableFilter ;

  public slots:

    void      commitAndClose               (QAbstractItemDelegate::EndEditHint hint = QAbstractItemDelegate::NoHint) ;
    void      cancelAndClose               (QAbstractItemDelegate::EndEditHint hint = QAbstractItemDelegate::NoHint) ;

  protected slots:

  private slots:

    void      comboxBoxItemActivated       (int index) ;
    void      listWidgetCurrentItemChanged (QListWidgetItem * current    ,
                                            QListWidgetItem * previous ) ;
    void      lineEditReturnPressed        (void) ;

};

class Q_AIV_EXPORT AbstractFilterProxyModel : public QSortFilterProxyModel
{
  Q_OBJECT
  public:

    explicit AbstractFilterProxyModel    (QObject* parent = 0) ;
    virtual ~AbstractFilterProxyModel    (void) ;

    AbstractFilterModel * filterModel    (void) const;

    QVariant              headerData     (int             section               ,
                                          Qt::Orientation orientation           ,
                                          int             role = Qt::DisplayRole) const;
    void                  setFilterModel (AbstractFilterModel * model      ) ;
    virtual void          setSourceModel (QAbstractItemModel  * sourceModel) ;

  protected:

    void emitResultCountChanged          (void) ;
    virtual bool filterAcceptsRow        (int                 source_row   ,
                                          const QModelIndex & source_parent) const = 0 ;

  private:

    void * D ;

  public slots:

  protected slots:

  private slots:

    void updateResult                    (void) ;

  signals:

    void resultCountChanged              (int filteredRows,int unfilteredRows) ;

};

class Q_AIV_EXPORT CheckStateProxyModel : public QIdentityProxyModel
{
  Q_OBJECT
  public:

    explicit              CheckStateProxyModel  (QObject * parent = 0) ;
    virtual              ~CheckStateProxyModel  (void) ;

    int                   checkableColumnsCount (void) const ;
    QModelIndexList       checkedIndexes        (void) const ;
    virtual QVariant      data                  (const QModelIndex & index,int role = Qt::DisplayRole ) const ;
    virtual Qt::ItemFlags flags                 (const QModelIndex & index) const ;
    bool                  isChecked             (const QModelIndex & index) const ;
    bool                  isColumnCheckable     (int column) const ;
    void                  setColumnCheckable    (int column,bool checkable = true) ;
    void                  setCheckedIndexes     (const QModelIndexList & indexes) ;
    void                  setCheckedValues      (int column,const QVariantList & values) ;
    virtual bool          setData               (const QModelIndex & index,const QVariant & value,int role) ;
    virtual void          setSourceModel        (QAbstractItemModel * sourceModel) ;

  protected:

    void * D ;

  public slots:

    void setAllChecked (bool checked = true) ;
    void setChecked    (const QModelIndex & index,bool checked) ;

  protected slots:

    void sourceModelRowsAboutToBeRemoved (const QModelIndex & parent,int start,int end) ;
    void sourceModelAboutToBeReset       (void) ;

};

class Q_AIV_EXPORT ConditionalDecorationProxyModel : public QSortFilterProxyModel
{
  Q_OBJECT
  Q_PROPERTY(QSize iconSize    READ iconSize    WRITE setIconSize)
  Q_PROPERTY(int   iconSpacing READ iconSpacing WRITE setIconSpacing)
  public:

    enum ItemDataRole                                {
      ConditionalDecorationRole = Qt::UserRole + 421 ,
      IconSetsRole                                 } ;

    explicit ConditionalDecorationProxyModel (QObject * parent = 0) ;
    virtual ~ConditionalDecorationProxyModel (void) ;

    void             addDecoration  (int column,AbstractItemModelDecoration * decoration) ;
    void             addIcon        (const QString & set,const QString & name,const QString & resource) ;
    void             addIconSet     (const QString & name,const QVariantMap icons) ;
    virtual QVariant data           (const QModelIndex & index,int role) const ;
    QIcon            icon           (const QString & set,const QString & name) const ;
    QSize            iconSize       (void) const ;
    int              iconSpacing    (void) const ;
    WMAPs            iconSets       (void) const ;
    bool             restoreState   (const QByteArray & state) ;
    QByteArray       stateState     (void) const ;
    virtual bool     setData        (const QModelIndex & index,const QVariant & value,int role) ;
    void             setIconSize    (const QSize & size) ;
    void             setIconSpacing (int spacing) ;

  private:

    void * D ;

};

class Q_AIV_EXPORT GroupingProxyModel : public QAbstractItemModel
{
  Q_OBJECT
  Q_PROPERTY(int modelColumn READ modelColumn WRITE setModelColumn)
  public:

    explicit GroupingProxyModel(QObject *parent = 0);
    virtual ~GroupingProxyModel(void) ;

    void addGroup(const QString & text, const QVariant & value = QVariant());
    void addGroup(const QIcon & icon, const QString & text, const QVariant & value = QVariant());
    int columnCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & proxyIndex, int role) const;
    Qt::ItemFlags flags(const QModelIndex & index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;

    int findText(const QString & text) const;
    QModelIndex mapFromSource(const QModelIndex & sourceIndex) const;
    QModelIndex mapToSource(const QModelIndex & proxyIndex) const;
    int modelColumn() const;
    QModelIndex parent(const QModelIndex & child) const;
    bool removeGroup(int index);
    bool restoreGroups(const QByteArray & data);
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QByteArray saveGroups() const;
    bool setData(const QModelIndex & index, const QVariant &value, int role);
    void setModelColumn(int column);
    virtual void setSourceModel( QAbstractItemModel* sourceModel );

  protected:

    void buildGroups(void);

    int groupAt(int sourceModelRow);
    void moveRows(int row, int count);
    void removeSourceModelRow(int sourceModelRow);

    void * D ;

  public slots:

  protected slots:

  private slots:

    void dataChangedHandler(const QModelIndex & topLeft, const QModelIndex & bottomRight);
    void rowsAboutToBeInsertedHandler( const QModelIndex & parent, int start, int end );
    void rowsInsertedHandler(const QModelIndex & parent, int first, int last);
};

class Q_AIV_EXPORT SharedItemSelectionModel : public QItemSelectionModel
{
  Q_OBJECT
  public:

    explicit SharedItemSelectionModel (QAbstractItemModel  * model        ,
                                       QItemSelectionModel * shared       ,
                                       QObject             * parent = 0 ) ;
    virtual ~SharedItemSelectionModel (void) ;

  protected:

    void * D ;

  public slots:

    void select(const QModelIndex & index, QItemSelectionModel::SelectionFlags command);
    void select(const QItemSelection & selection, QItemSelectionModel::SelectionFlags command);

  protected slots:

    void sharedSelectionChanged(const QItemSelection & selected, const QItemSelection & deselected);

};

class Q_AIV_EXPORT SingleColumnProxyModel : public QSortFilterProxyModel
{
  Q_OBJECT
  Q_PROPERTY(int sourceModelColumn READ sourceModelColumn WRITE setSourceModelColumn)
  public:

    explicit SingleColumnProxyModel (QObject *parent = 0) ;
    virtual ~SingleColumnProxyModel (void) ;

    int sourceModelColumn(void) const ;

  protected:

    void * D ;

    bool filterAcceptsColumn(int source_column, const QModelIndex & source_parent) const;

  public slots:

    void setSourceModelColumn(int column) ;

};

class Q_AIV_EXPORT UniqueValuesProxyModel : public QSortFilterProxyModel
{
  Q_OBJECT
  Q_PROPERTY(bool emptyItemsAllowed READ emptyItemsAllowed WRITE setEmptyItemsAllowed)
  Q_PROPERTY(int modelColumn READ modelColumn WRITE setModelColumn)
  public:

    explicit UniqueValuesProxyModel (QObject * parent = 0) ;
    virtual ~UniqueValuesProxyModel (void) ;

    QVariant data(const QModelIndex &index, int role) const;
    bool emptyItemsAllowed() const;
    bool filterAcceptsRow(int source_row, const QModelIndex & source_parent) const;
    virtual bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex());
    int modelColumn() const;
    void setEmptyItemsAllowed(bool on);
    void setModelColumn(int colum);
    void setSourceModel(QAbstractItemModel* sourceModel);

  protected:

     void * D ;

  public slots:

  protected slots:

  private slots:

    bool isDuplicate(int row) const;
    void buildMap();
    void sourceModelDataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight);

  signals:

    void progressChanged(int progress);
};

class Q_AIV_EXPORT AbstractFilterModel : public QAbstractItemModel
{
  Q_OBJECT
  public:

    struct FilterTypeEntry {
      int     type         ;
      QIcon   icon         ;
      QString text         ;
      QString tooltip      ;
    }                      ;

    enum ItemDataRole                           {
      ValueFilterTypeRole   = Qt::UserRole + 42 ,
      DefaultFilterTypeRole                     ,
      ColumnFilterTypesRole                   } ;

    explicit                   AbstractFilterModel (QObject * parent = 0) ;
    virtual                   ~AbstractFilterModel (void) ;

    int                        columnCount    (const QModelIndex & parent = QModelIndex()) const ;
    virtual AbstractFilter *   createFilter   (const QModelIndex & index, const QVariantMap & properties) const = 0 ;
    QVariant                   data           (const QModelIndex & index, int role = Qt::DisplayRole ) const ;
    FilterGroup            *   filterGroup    (const QModelIndex & index) const ;
    FilterGroup            *   groupAt        (int row) const ;
    AbstractFilter         *   filter         (const QModelIndex & index) const ;
    QList<AbstractFilter   * > filtersAtRow   (int row) const ;
    Qt::ItemFlags              flags          (const QModelIndex & index) const ;
    QVariant                   headerData     (int             section               ,
                                               Qt::Orientation orientation           ,
                                               int             role = Qt::DisplayRole) const;
    QModelIndex                index          (int                 row                   ,
                                               int                 column                ,
                                               const QModelIndex & parent = QModelIndex()) const;
    bool                       insertRows     (int                 row                     ,
                                               int                 count                   ,
                                               const QModelIndex & parent = QModelIndex()) ;

    AdvancedItemViews::FilterMatchMode matchMode    (void) const ;
    AdvancedItemViews::FilterProxyMode mode         (void) const ;
    QList<FilterTypeEntry>    registeredFilterTypes (void) const;

    QModelIndex                parent         (const QModelIndex & index) const ;
    bool                       removeRows     (int                 row                     ,
                                               int                 count                   ,
                                               const QModelIndex & parent = QModelIndex()) ;
    int                        rowCount       (const QModelIndex & parent = QModelIndex()) const ;
    bool                       setData        (const QModelIndex & index                ,
                                               const QVariant    & value                ,
                                               int                 role = Qt::EditRole) ;
    void                       setDefaultFilterType (int column,int type) ;
    bool                       setHeaderData  (int              section               ,
                                               Qt::Orientation  orientation           ,
                                               const QVariant & value                 ,
                                               int              role = Qt::EditRole ) ;
    void                       setMatchMode   (AdvancedItemViews::FilterMatchMode flag) ;
    void                       setMode        (AdvancedItemViews::FilterProxyMode mode);
    void                       setSourceModel (QAbstractItemModel * model);
    QAbstractItemModel *       sourceModel    (void) const;

  protected:

    bool registerFilter (int type,const QString & text,const QString & toolTip = QString()) ;
    bool registerFilter (int type,const QIcon   & icon,const QString & text,const QString & toolTip = QString()) ;

  private:

    void * D;

  public slots:

  protected slots:

  private slots:

    void sourceModelLayoutChanged (void) ;

  signals:

    void matchModeChanged         (AdvancedItemViews::FilterMatchMode mode) ;
    void modeChanged              (AdvancedItemViews::FilterProxyMode mode) ;

};

class Q_AIV_EXPORT AbstractFilter
{
  public:

    static const int UserType = 65536 ;
    enum { Type = -1 } ;

    explicit               AbstractFilter (void) ;
    explicit               AbstractFilter (const QMap<QString,QVariant> & properties) ;
    virtual               ~AbstractFilter (void) ;

    int                    type                  (void) const ;
    int                    column                (void) const ;
    virtual void           addContextMenuActions (QMenu   * menu       ,
                                                  QWidget * receiver ) ;
    virtual QWidget *      createEditor          (FilterViewItemDelegate     * delegate ,
                                                  QWidget                    * parent   ,
                                                  const QStyleOptionViewItem & option   ,
                                                  const QModelIndex          & index    ) const = 0 ;
    virtual QVariant       data                  (int role = Qt::DisplayRole) const = 0 ;
    QColor                 highlightColor        (void) const ;
    bool                   isEnabled             (void) const ;
    bool                   isValid               (void) const ;
    virtual bool           matches               (const QVariant & value,int type = -1) const = 0 ;
    QVariant               property              (const QString  & key                      ,
                                                  const QVariant & defaultValue = QVariant()) const ;
    QMap<QString,QVariant> properties            (void) const ;
    int                    row                   (void) const ;
    virtual void           setEditorData         (QWidget * editor,const QModelIndex & index) = 0 ;
    void                   setHighlightColor     (const QColor & color) ;
    virtual void           setModelData          (QWidget            * editor ,
                                                  QAbstractItemModel * model  ,
                                                  const QModelIndex  & index) = 0;
    void                   setEnabled            (bool on) ;
    void                   setProperty           (const QString  & name    ,
                                                  const QVariant & value ) ;

    virtual void           updateEditorGeometry  (QWidget                    * editor ,
                                                  const QStyleOptionViewItem & option ,
                                                  const QModelIndex          & index  ) = 0 ;

  protected:

    AbstractFilter(int type, int row, int column) ;

  private:

    friend class AbstractFilterModel ;

    void setRow(int row);

    void * D ;

};

template <class T> inline T qfilter_cast(AbstractFilter * filter)
{
  return int(static_cast<T>(0)->Type) == int(AbstractFilter::Type)
    || (filter && int(static_cast<T>(0)->Type) == filter->type()) ? static_cast<T>(filter) : 0 ;
}

template <class T> inline T qfilter_cast(const AbstractFilter * filter)
{
  return int(static_cast<T>(0)->Type) == int(AbstractFilter::Type)
    || (filter && int(static_cast<T>(0)->Type) == filter->type()) ? static_cast<T>(filter) : 0 ;
}

class Q_AIV_EXPORT FilterGroup
{
  public:

    explicit                FilterGroup    (void) ;
    explicit                FilterGroup    (const QString & name) ;
    virtual                ~FilterGroup    (void) ;

    void                    add            (AbstractFilter * filter) ;
    int                     count          (void) const ;
    AbstractFilter       *  filter         (int index) const ;
    AbstractFilter       *  filterAtColumn (int column) const ;
    QList<AbstractFilter *> filters        (void) const ;
    bool                    hasFilter      (int index) const ;
    QString                 name           (void) const ;
    bool                    remove         (int column) ;
    void                    setName        (const QString & name) ;

  protected:

    QList<AbstractFilter *> cFilterList ;
    bool                    cEnabled    ;
    QString                 cName       ;

  private:

};

class Q_AIV_EXPORT AbstractItemModelDecoration
{
  public:

    explicit AbstractItemModelDecoration (void) ;
    virtual ~AbstractItemModelDecoration (void) ;

    virtual QVariant decorate      (const QModelIndex & index,int role = Qt::DecorationRole) const = 0 ;
    QIcon            defaultIcon   (void) const ;
    QVariant         property      (const QString & key,const QVariant & defaultValue = QVariant()) const ;
    WMAPs            properties    (void) const ;
    bool             restoreState  (const QByteArray & state) ;
    QByteArray       saveState     (void) const ;
    void             setProperty   (const QString & name,const QVariant & value) ;
    void             setProperties (const QVariantMap & properties);

  protected:

    void * D ;

};

class Q_AIV_EXPORT ConditionalDecoration : public AbstractItemModelDecoration
{
  public:

    enum MatchFlag          {
      MatchFlagInvalid = -1 ,
      Contains         =  1 , /**< The data provided by the item contains the value. */
      ContainsNot           , /**< The data provided by the item does not contain the value. */
      EndsWith              , /**< The data provided by the item ends with the value. */
      EndsNotWith           , /**< The data provided by the item does not end with the value. */
      IsBetween             , /**< The data provided by the item is between the range specified. */
      IsEmpty               , /**< The item contains empty data. */
      IsEqual               , /**< The data provided by the item is equal to the value. */
      IsGreater             , /**< The data provided by the item is greater than the value. */
      IsGreaterOrEqual      , /**< The data provided by the item is greater or equal to value. */
      IsLessOrEqual         , /**< The data provided by the item is less or equal to the value. */
      IsLess                , /*!< The data provided is less than the value. */
      IsNotBetween          , /**< The data provided by the item is not between the specified range. */
      IsNotEmpty            , /**< The item contains data. */
      IsNotEqual            , /**< The data provided by the item is not equal to the value. */
      StartsWith            , /*!< The data provided by the item starts with the search term. */
      StartsNotWith           /**< The data provided by the item does not start the value. */
    }                       ;

    explicit ConditionalDecoration   (void) ;
    explicit ConditionalDecoration   (int column) ;
    virtual ~ConditionalDecoration   (void) ;

    ConditionalDecoration::MatchFlag matchFlag(int index) const;
    void        addCondition         (ConditionalDecoration::MatchFlag matchFlag, const QVariant & value, const QString & set, const QString & name);
    int         count                (void) const;
    QVariant    decorate             (const QModelIndex & index, int role = Qt::DecorationRole) const;
    QString     iconName             (int index) const;
    bool        matches              (const QModelIndex & index, const QVariantMap & properties) const;
    QVariantMap properties           (int index) const;
    void        setHighlightRole     (int role);
    void        setDefaultDecoration (const QString & set, const QString & name);
    QVariant    value                (int index) const;

  protected:

    bool equal              (const QVariant & left,const QVariant & right) const ;
    bool greaterOrEqualThan (const QVariant & left,const QVariant & right) const ;
    bool greaterThan        (const QVariant & left,const QVariant & right) const ;
    bool lessOrEqualThan    (const QVariant & left,const QVariant & right) const ;
    bool lessThan           (const QVariant & left,const QVariant & right) const ;

};

class Q_AIV_EXPORT AdvancedHeaderView : public HeaderView
{
  Q_OBJECT
  public:

    explicit AdvancedHeaderView (Qt::Orientation orientation,QWidget * parent = NULL,Plan * plan = NULL) ;
    virtual ~AdvancedHeaderView (void);

  protected:

    virtual void contextMenuEvent( QContextMenuEvent * event );

  private:

  public slots:

    void showAllColumns();

  protected slots:

    void addFilterSetActionTriggered();
    void hideFilterActionTriggered();
    void filterModeActionTriggered();
    void hideSectionActionTriggered();
    void highlightModeActionTriggered();
    void invertedMatchModeActionTriggered();
    void moreColumnsActionTriggered();
    void removeFilterSetActionTriggered();
    void renameFilterActionTriggered();
    void sectionToggled( bool on );
    void showFilterActionTriggered();
    void toggleSortOrderActionTriggered();

  private slots:

  signals:

};

class Q_AIV_EXPORT FixedRowsFilterProxyModel : public QSortFilterProxyModel
{
  Q_OBJECT
  public:

    explicit FixedRowsFilterProxyModel (QObject * parent = 0) ;
    virtual ~FixedRowsFilterProxyModel (void) ;

    void clear();
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool isEnabled() const;
    bool isRowPinned(int row) const;

  protected:

    void * D ;

  public slots:

    void setEnabled(bool on);
    void sourceModelReset();
    void toggleRow(const QModelIndex & index);
    void setRowFixed(const QModelIndex & index, bool fixed);

  protected slots:

  private slots:

  signals:

};

class Q_AIV_EXPORT FixedRowsDecorationProxyModel : public QIdentityProxyModel
{
  Q_OBJECT
  public:

    explicit FixedRowsDecorationProxyModel (FixedRowsFilterProxyModel * filterProxy, QObject* parent = 0) ;
    virtual ~FixedRowsDecorationProxyModel (void);

    QVariant headerData (int section, Qt::Orientation orientation, int role) const;
    QSize    iconSize   (void) const;
    bool     isEnabled  (void) const;

  protected:

    void * D ;

  private:

  public slots:

    void setEnabled(bool on);
    void toggleRow(const QModelIndex & index);

  protected slots:

  private slots:

  signals:

    void modelToggled(bool on);

};

class Q_AIV_EXPORT AutoFilterEditorPopup : public FilterEditorPopupWidget
{
  Q_OBJECT
  public:

    explicit AutoFilterEditorPopup (StandardConstructor) ;
    virtual ~AutoFilterEditorPopup (void) ;

    int mode() const;
    QVariantList selectedValues(int role = Qt::DisplayRole) const;
    void setSelectedValues(const QVariantList & values);
    void setSourceModel(QAbstractItemModel* model, int column);

  protected:

    QToolButton            * m_emptyToolButton    ;
    LineEdit               * m_lineEdit           ;
    ListView               * m_listView           ;
    int                      m_mode               ;
    QToolButton            * m_notEmptyToolButton ;
    QProgressDialog        * m_progress           ;
    QCheckBox              * m_selectCheckBox     ;
    CheckStateProxyModel   * m_checkStateProxy    ;
    SingleColumnProxyModel * m_singleColumnProxy  ;
    UniqueValuesProxyModel * m_singleValueProxy   ;

  private:

  public slots:

  protected slots:

  private slots:

    void uniqueValueModelProgressChanged(int progress);
    void checkStateProxyDataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight);
    void emptyToolButtonClicked();
    void notEmptyToolButtonClicked();
    void searchForTextEdited(const QString & text);
    void selectCheckBoxStateChanged(int state);

  signals:

    void modeChanged();

};

class Q_AIV_EXPORT AutoFilterEditor : public FilterEditorWidget
{
  Q_OBJECT
  public:

    explicit AutoFilterEditor(StandardConstructor) ;
    virtual ~AutoFilterEditor(void) ;

    QVariantList selectedValues(int role = Qt::DisplayRole) const;
    void setSourceModel(QAbstractItemModel* model, int column);

  protected:

  private:

  public slots:

    void modeSelected();

  protected slots:

  private slots:

  signals:
};

class Q_AIV_EXPORT AutoFilter : public AbstractFilter
{
  public:

    enum {
        Type = 0
    };

    explicit AutoFilter (int row, int column) ;
    virtual ~AutoFilter (void) ;

    QWidget* createEditor(FilterViewItemDelegate * delegate, QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QVariant data(int role = Qt::DisplayRole) const;
    bool matches(const QVariant & value, int type) const;
    void setEditorData(QWidget* editor, const QModelIndex & index);
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex & index);
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem & option, const QModelIndex & index);

};

class Q_AIV_EXPORT RangeFilterEditorPopup : public FilterEditorPopupWidget
{
  Q_OBJECT
  public:

    explicit RangeFilterEditorPopup (StandardConstructor) ;
    virtual ~RangeFilterEditorPopup (void) ;

    QString rangeFrom() const;
    QString rangeTo() const;
    void setRangeFrom( const QString & text );
    void setRangeTo( const QString & text );

  protected:

    LineEdit * m_rangeFrom ;
    LineEdit * m_rangeTo   ;

  private:

  public slots:

  protected slots:

  private slots:

  signals:

};

class Q_AIV_EXPORT RangeFilterEditor : public FilterEditorWidget
{
  Q_OBJECT
  public:

    explicit RangeFilterEditor (StandardConstructor) ;
    virtual ~RangeFilterEditor (void) ;

    QString rangeFrom() const;
    QString rangeTo() const;
    void setRangeFrom( const QString & text );
    void setRangeTo( const QString & text );

  protected:

    bool eventFilter(QObject *obj, QEvent *event) ;

  private:

  public slots:

  protected slots:

  private slots:

  signals:

};

class Q_AIV_EXPORT RangeFilter : public AbstractFilter
{
  public:

    enum {
        Type = 2
    };

    explicit RangeFilter (int row, int column) ;
    virtual ~RangeFilter (void) ;

    QWidget* createEditor(FilterViewItemDelegate * delegate, QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QVariant data(int role = Qt::DisplayRole) const;
    bool matches(const QVariant & value, int type) const;
    void setEditorData(QWidget * editor, const QModelIndex & index);
    void setModelData(QWidget* editor, QAbstractItemModel * model, const QModelIndex & index);
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem & option, const QModelIndex & index);

};

class Q_AIV_EXPORT RegExpFilterEditor : public Widget
{
  Q_OBJECT
  public:

    explicit RegExpFilterEditor (StandardConstructor) ;
    virtual ~RegExpFilterEditor (void) ;

    Qt::CaseSensitivity caseSenstivity() const;
    QString pattern() const;
    QRegExp::PatternSyntax patternSyntax() const;
    void setCaseSensitivity(Qt::CaseSensitivity caseSenstivity);
    void setPattern(const QString & pattern);
    void setPatternSyntax(QRegExp::PatternSyntax patternSyntax);

  private:

    Qt::CaseSensitivity m_sensitivity      ;
    LineEdit          * m_patternEdit      ;
    QCheckBox         * m_wildcardCheckBox ;
    ClickableLabel    * m_sensitivityLabel ;

  public slots:

  protected slots:

  private slots:

    void sensitivityLabelClicked(Qt::MouseButtons buttons);

  signals:

};

class Q_AIV_EXPORT RegExpFilter : public AbstractFilter
{
  public:

    enum {
        Type = 3
    };

    explicit RegExpFilter(int row,int column) ;
    virtual ~RegExpFilter(void) ;

    QWidget* createEditor(FilterViewItemDelegate * delegate, QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QVariant data(int role = Qt::DisplayRole) const;
    bool matches(const QVariant & value, int type) const;
    QRegExp regExp() const;
    void setEditorData(QWidget * editor, const QModelIndex & index);
    void setModelData(QWidget* editor, QAbstractItemModel * model, const QModelIndex & index);
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem & option, const QModelIndex & index);

};

class Q_AIV_EXPORT SelectionListFilterEditorPopup : public FilterEditorPopupWidget
{
  Q_OBJECT
  public:

    explicit SelectionListFilterEditorPopup (StandardConstructor) ;
    virtual ~SelectionListFilterEditorPopup (void);

    bool cancelAndClose(QObject* obj, int key) const;
    bool commitAndClose(QObject* obj, int key) const;
    int mode() const;
    QVariantList selectedValues() const;
    void setSourceModel(QAbstractItemModel* model, int column);
    void setSelectedValues(const QVariantList & selectedValues);
    void setValues(const QVariantList & values);
    QVariantList values(int role = Qt::DisplayRole) const;

  protected:

    CheckStateProxyModel * m_checkStateProxy    ;
    QToolButton          * m_emptyToolButton    ;
    LineEdit             * m_lineEdit           ;
    ListView             * m_listView           ;
    int                    m_mode               ;
    QToolButton          * m_notEmptyToolButton ;
    QCheckBox            * m_selectCheckBox     ;
    QStandardItemModel   * m_model              ;

  private:

  public slots:

  protected slots:

  private slots:

    void checkStateProxyDataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight);
    void emptyToolButtonClicked();
    void notEmptyToolButtonClicked();
    void searchForTextEdited(const QString & text);
    void selectCheckBoxStateChanged(int state);

  signals:

    void modeChanged();

};

class Q_AIV_EXPORT SelectionListFilterEditor : public FilterEditorWidget
{
  Q_OBJECT
  public:

    explicit SelectionListFilterEditor (StandardConstructor) ;
    virtual ~SelectionListFilterEditor (void) ;

  protected:

  private:

  public slots:

    void modeSelected();

  protected slots:

  private slots:

  signals:

};

class Q_AIV_EXPORT SelectionListFilter : public AbstractFilter
{
  public:

    enum {
        Type = 6
    };

    enum DataSource {
      Model,
      Filter
    };

    explicit SelectionListFilter(int row,int column) ;
    virtual ~SelectionListFilter(void) ;

    QWidget* createEditor(FilterViewItemDelegate * delegate, QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QVariant data(int role = Qt::DisplayRole) const;
    SelectionListFilter::DataSource dataSource() const;
    bool matches(const QVariant & value, int type) const;
    void setDataSource(SelectionListFilter::DataSource source);
    void setEditorData(QWidget * editor, const QModelIndex & index);
    void setModelData(QWidget* editor, QAbstractItemModel * model, const QModelIndex & index);
    void setValues(const QVariantList & values);
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem & option, const QModelIndex & index);

};

class Q_AIV_EXPORT SpecialFilterEditor : public FilterEditorWidget
{
  Q_OBJECT
  public:

    explicit SpecialFilterEditor (StandardConstructor) ;
    virtual ~SpecialFilterEditor (void) ;

    Qt::CaseSensitivity caseSensitivity() const;
    Qt::MatchFlag matchFlag() const;
    void setCaseSensitivity(Qt::CaseSensitivity sensitivity);
    void setMatchFlag(Qt::MatchFlag flag);
    void setText(const QString & text);
    QString text() const;

  protected:

    Qt::CaseSensitivity cSensitivity      ;
    Qt::MatchFlag       cMatchFlag        ;
    LineEdit          * cValueLineEdit    ;
    ClickableLabel    * cSensitivityLabel ;
    ClickableLabel    * cMatchFlagsLabel  ;

  private:

  public slots:

  protected slots:

  private slots:

    void sensitivityLabelClicked(Qt::MouseButtons buttons);
    void matchFlagsLabelClicked(Qt::MouseButtons buttons);

  signals:

};

class Q_AIV_EXPORT SpecialFilter : public AbstractFilter
{
  public:

    enum {
        Type = 7
    };
        //
    enum MatchFlag {
        MatchFlagInvalid = -1,
        Contains = 1,
        ContainsNot,
        EndsWith,
        EndsNotWith,
        IsBetween,
        IsEmpty,
        IsEqual,
        IsGreator,
        IsGreatorOrEqual,
        IsLessOrEqual,
        IsLess,
        IsNotBetween,
        IsNotEmpty,
        IsNotEqual,
        StartsWith,
        StartsNotWith
    };

    explicit SpecialFilter(int row, int column) ;
    virtual ~SpecialFilter(void) ;

    QWidget * createEditor(FilterViewItemDelegate* delegate, QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    void addContextMenuActions(QMenu* menu, QWidget* receiver);
    QVariant data(int role = Qt::DisplayRole) const;
    bool matches(const QVariant & value, int type) const;
    SpecialFilter::MatchFlag matchFlag() const;
    void setEditorData(QWidget * editor, const QModelIndex & index);
    void setModelData(QWidget* editor, QAbstractItemModel * model, const QModelIndex & index);
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem & option, const QModelIndex & index);

};

class Q_AIV_EXPORT TextFilterEditor : public Widget
{
  Q_OBJECT
  public:

    explicit TextFilterEditor (StandardConstructor) ;
    virtual ~TextFilterEditor (void) ;

    Qt::CaseSensitivity caseSensitivity() const;
    Qt::MatchFlag matchFlag() const;
    void setCaseSensitivity(Qt::CaseSensitivity sensitivity);
    void setMatchFlag(Qt::MatchFlag flag);
    void setText(const QString & text);
    QString text() const;

  protected:

    Qt::CaseSensitivity cSensitivity      ;
    Qt::MatchFlag       cMatchFlag        ;
    LineEdit          * cValueLineEdit    ;
    ClickableLabel    * cSensitivityLabel ;
    ClickableLabel    * cMatchFlagsLabel  ;

  private:

  public slots:

  protected slots:

  private slots:

    void sensitivityLabelClicked(Qt::MouseButtons buttons);
    void matchFlagsLabelClicked(Qt::MouseButtons buttons);

  signals:

};

class Q_AIV_EXPORT TextFilter : public AbstractFilter
{
  public:

    enum {
        Type = 4
    };

    explicit TextFilter (int row, int column) ;
    virtual ~TextFilter (void) ;

    QWidget* createEditor(FilterViewItemDelegate * delegate, QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    void addContextMenuActions(QMenu* menu, QWidget* receiver);
    QVariant data(int role = Qt::DisplayRole) const;
    bool matches(const QVariant & value, int type) const;
    void setEditorData(QWidget * editor, const QModelIndex & index);
    void setModelData(QWidget* editor, QAbstractItemModel * model, const QModelIndex & index);
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem & option, const QModelIndex & index);

};

class Q_AIV_EXPORT ValueFilterEditor : public Widget
{
  Q_OBJECT
  public:

    explicit ValueFilterEditor (StandardConstructor) ;
    virtual ~ValueFilterEditor (void) ;

    Qt::CaseSensitivity caseSensitivity() const;
    Qt::MatchFlag matchFlag() const;
    void setCaseSensitivity(Qt::CaseSensitivity sensitivity);
    void setMatchFlag(Qt::MatchFlag flag);
    void setText(const QString & text);
    QString text() const;

  protected:

    Qt::CaseSensitivity m_sensitivity      ;
    Qt::MatchFlag       m_matchFlag        ;
    LineEdit          * m_valueLineEdit    ;
    ClickableLabel    * m_sensitivityLabel ;
    ClickableLabel    * m_matchFlagsLabel  ;

  private:

  public slots:

  protected slots:

  private slots:

    void sensitivityLabelClicked(Qt::MouseButtons buttons);
    void matchFlagsLabelClicked(Qt::MouseButtons buttons);

  signals:

};

class Q_AIV_EXPORT ValueFilter : public AbstractFilter
{
  public:

    enum {
        Type = 5
    };

    explicit ValueFilter (int row, int column) ;
    virtual ~ValueFilter (void) ;

    QWidget* createEditor(FilterViewItemDelegate * delegate, QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QVariant data(int role = Qt::DisplayRole) const;
    bool matches(const QVariant & value, int type) const;
    void setEditorData(QWidget * editor, const QModelIndex & index);
    void setModelData(QWidget* editor, QAbstractItemModel * model, const QModelIndex & index);
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem & option, const QModelIndex & index);

};

class Q_AIV_EXPORT FilterModel : public AbstractFilterModel
{
  Q_OBJECT
  public:

    explicit FilterModel (QObject * parent) ;
    virtual ~FilterModel (void);

  protected:

    AbstractFilter * createFilter(const QModelIndex & index, const QVariantMap & properties) const ;

};

class Q_AIV_EXPORT FixedRowsTableView : public TableView
{
  Q_OBJECT
  Q_PROPERTY(bool fixedRowsMode READ fixedRowsMode WRITE setFixedRowsMode)
  public:

    explicit FixedRowsTableView (StandardConstructor) ;
    virtual ~FixedRowsTableView (void) ;

    FixedRowsDecorationProxyModel* decorationProxy() const;
    bool fixedRowsMode() const;
    FixedRowsFilterProxyModel* filterProxy() const;
    void setModel(QAbstractItemModel* model);

  public slots:

    void setFixedRowsMode(bool on) ;

  protected:

    virtual void closeEditor(QWidget* editor, QAbstractItemDelegate::EndEditHint hint);
    virtual void focusInEvent(QFocusEvent* event);

  private:

    void * D ;

  public slots:

  protected slots:

  private slots:

    void updateHeight();

    void verticalHeaderSectionClicked(int section);

  signals:

    void focusReceived();

};

class Q_AIV_EXPORT FilterViewConnector : public QObject
{
  Q_OBJECT
  public:

    explicit FilterViewConnector(FilterView* filterView);
    explicit FilterViewConnector(FilterView* filterView,QTableView * tableView , QObject * parent = 0) ;
    explicit FilterViewConnector(FilterView* filterView,QTreeView  * treeView  , QObject * parent = 0) ;
    virtual ~FilterViewConnector(void) ;

    void setDataView (QTableView * view) ;
    void setDataView (QTreeView  * view) ;

  protected:

    FilterView * cFilterView ;
    QTableView * cTableView  ;
    QTreeView  * cTreeView   ;

  private:

  public slots:

    void adjustVerticalHeaderWidth();

  protected slots:

    void dataViewHorizontalScrollBarRangeChanged(int min, int max);
    void dataViewHorizontalScrollBarSilderMoved(int value);
    void dataViewHorizontalSortIndicatorChanged(int logicalIndex, Qt::SortOrder order);
    void dataViewHorizontalSectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex);
    void dataViewHorizontalSectionResized(int logicalIndex, int oldSize, int newSize);
    void dataViewVerticalScrollBarRangeChanged(int min, int max);

    void filterViewHorizontalSectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex);
    void filterViewHorizontalSectionResized(int logicalIndex, int oldSize, int newSize);
    void filterViewHorizontalSortIndicatorChanged(int logicalIndex, Qt::SortOrder order);

  private slots:

  signals:

};

class Q_AIV_EXPORT FilterView : public TableView
{
  Q_OBJECT
  Q_PROPERTY(int maxVisibileFilterSets READ maxVisibileFilterSets WRITE setMaxVisibileFilterSets)
  public:

    explicit FilterView (StandardConstructor) ;
    virtual ~FilterView (void);

    void connectToView(QAbstractItemView* view);
    int defaultFilterType() const;
    FilterViewConnector* filterViewConnector() const;
    bool filterVisible() const;
    int maxVisibileFilterSets() const;
    void setAllowedFilterTypes(int types);
    void setDefaultFilterType(int type);
    void setMaxVisibileFilterSets(int rows);
    void setModel(QAbstractItemModel* model);
    void toggleFilter( const QModelIndex & index );

  protected:

    virtual void contextMenuEvent( QContextMenuEvent* event );
    virtual void focusInEvent(QFocusEvent* event);
    virtual void mousePressEvent( QMouseEvent* event );

  private:

    void * D ;

    void updateGeometry();

  public slots:

    void addFilter();
    void changeColor();
    void changeProperties();
    void disableSelectedFilters();
    void enableSelectedFilters();
    void removeFilter();
    void setFilterVisible(bool visible);
    void toggleSelectedFilters();

  protected slots:

    void hideFilter();
    void showFilter();

  private slots:

  signals:

    void calcGeometryRequested();
    void cornerButtonClicked();
    void focusReceived();
    void visibilityChanged(bool visible);
    void removeColumnFilter(int row, int column);
};

}

QT_END_NAMESPACE

#endif
