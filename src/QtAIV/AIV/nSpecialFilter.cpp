#include <qtaiv.h>

N::SpecialFilter:: SpecialFilter  (int row, int column)
                 : AbstractFilter (N::SpecialFilter::Type, row, column)
{
    setProperty("caseSensitivity", Qt::CaseInsensitive);
    setProperty("matchFlag", Qt::MatchContains);
}

N::SpecialFilter::~SpecialFilter(void)
{
}

QWidget * N::SpecialFilter::createEditor(FilterViewItemDelegate* delegate, QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    return new SpecialFilterEditor(parent);
}

void N::SpecialFilter::addContextMenuActions(QMenu* menu, QWidget* receiver)
{
    QVariantMap mDefaultProperties;
    QVariantMap mPropertiesToChange;
    mDefaultProperties["row"] = property("row").toInt();
    mDefaultProperties["column"] = property("column").toInt();

    QAction* mAction = 0;
    mAction = menu->addAction(QIcon(":/qaiv/filter/case_insensitive"), QObject::tr("Case insensitive"), receiver, SLOT(changeProperties()));
    mAction->setCheckable(true);
    mAction->setChecked(property("caseSensitivity").toInt() == Qt::CaseInsensitive);
    mPropertiesToChange = QVariantMap(mDefaultProperties);
    mPropertiesToChange["caseSensitivity"] = Qt::CaseInsensitive;
    mAction->setData(mPropertiesToChange);

    mAction = menu->addAction(QIcon(":/qaiv/filter/case_sensitive"), QObject::tr("Case sensitive"), receiver, SLOT(changeProperties()));
    mAction->setCheckable(true);
    mAction->setChecked(property("caseSensitivity").toInt() == Qt::CaseSensitive);
    mPropertiesToChange = QVariantMap(mDefaultProperties);
    mPropertiesToChange["caseSensitivity"] = Qt::CaseSensitive;
    mAction->setData(mPropertiesToChange);
    menu->addSeparator();

    mAction = menu->addAction(QIcon(":/qaiv/filter/starts_with"), QObject::tr("Match starts with"), receiver, SLOT(changeProperties()));
    mAction->setCheckable(true);
    mAction->setChecked(property("matchFlag").toInt() == Qt::MatchStartsWith);
    mPropertiesToChange = QVariantMap(mDefaultProperties);
    mPropertiesToChange["matchFlag"] = Qt::MatchStartsWith;
    mAction->setData(mPropertiesToChange);

    mAction = menu->addAction(QIcon(":/qaiv/filter/ends_with"), QObject::tr("Match ends with"), receiver, SLOT(changeProperties()));
    mAction->setCheckable(true);
    mAction->setChecked(property("matchFlag").toInt() == Qt::MatchEndsWith);
    mPropertiesToChange = QVariantMap(mDefaultProperties);
    mPropertiesToChange["matchFlag"] = Qt::MatchEndsWith;
    mAction->setData(mPropertiesToChange);

    mAction = menu->addAction(QIcon(":/qaiv/filter/contains"), QObject::tr("Match contains"), receiver, SLOT(changeProperties()));
    mAction->setCheckable(true);
    mAction->setChecked(property("matchFlag").toInt() == Qt::MatchContains);
    mPropertiesToChange = QVariantMap(mDefaultProperties);
    mPropertiesToChange["matchFlag"] = Qt::MatchContains;
    mAction->setData(mPropertiesToChange);
}

QVariant N::SpecialFilter::data(int role) const
{
    if (role == Qt::DisplayRole){
        return QString("%1").arg(property("value").toString());
    }
    return QVariant();
}

bool N::SpecialFilter::matches(const QVariant & value, int type) const
{
    Q_UNUSED(type);

    N::SpecialFilter::MatchFlag matchFlag;
    matchFlag = static_cast<N::SpecialFilter::MatchFlag>(property("matchFlag", Qt::MatchStartsWith).toInt());
        if (matchFlag == N::SpecialFilter::IsEmpty){
        return value.isNull();
        } else if (matchFlag == N::SpecialFilter::IsNotEmpty){
        return !value.isNull();
    }
        return false;
    //return QString::compare(value.toString().left(property("value").toString().length()), property("value").toString(), mSensitivity) == 0;
}

N::SpecialFilter::MatchFlag N::SpecialFilter::matchFlag() const
{
        return static_cast<N::SpecialFilter::MatchFlag>(property("matchFlag", Qt::MatchStartsWith).toInt());
}

void N::SpecialFilter::setEditorData(QWidget * editor, const QModelIndex & index)
{
    SpecialFilterEditor* mWidget = qobject_cast<SpecialFilterEditor*>(editor);
    if (mWidget){
        QVariantMap mProperties = index.data(Qt::EditRole).toMap();
        mWidget->setText(mProperties.value("value").toString());
        mWidget->setMatchFlag(static_cast<Qt::MatchFlag>(mProperties.value("matchFlag").toInt()));
        mWidget->setCaseSensitivity(static_cast<Qt::CaseSensitivity>(mProperties.value("caseSensitivity").toInt()));
    }
}

void N::SpecialFilter::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex & index)
{
    SpecialFilterEditor* mWidget = qobject_cast<SpecialFilterEditor*>(editor);
    if (mWidget){
        QVariantMap mProperties(index.data(Qt::EditRole).toMap());
        mProperties["value"] = mWidget->text();
        mProperties["caseSensitivity"] = mWidget->caseSensitivity();
        mProperties["matchFlag"] = mWidget->matchFlag();
        model->setData(index, mProperties);
    }
}

void N::SpecialFilter::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem & option, const QModelIndex & index)
{
}
