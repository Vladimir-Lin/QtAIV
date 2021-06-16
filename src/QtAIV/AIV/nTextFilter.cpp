#include <qtaiv.h>

N::TextFilter:: TextFilter     (int row, int column)
              : AbstractFilter (TextFilter::Type, row, column)
{
    setProperty("caseSensitivity", Qt::CaseInsensitive);
    setProperty("matchFlag", Qt::MatchContains);
}

N::TextFilter::~TextFilter(void)
{
}

QWidget* N::TextFilter::createEditor(FilterViewItemDelegate* delegate, QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    return new TextFilterEditor(parent);
}

void N::TextFilter::addContextMenuActions(QMenu* menu, QWidget* receiver)
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

QVariant N::TextFilter::data(int role) const
{
    if (role == Qt::DisplayRole){
        return QString("%1").arg(property("value").toString());
    }
    return QVariant();
}

bool N::TextFilter::matches(const QVariant & value, int type) const
{
    Q_UNUSED(type);

    Qt::CaseSensitivity mSensitivity;
    Qt::MatchFlag mFlag;
    mSensitivity = static_cast<Qt::CaseSensitivity>(property("caseSensitivity", Qt::CaseInsensitive).toInt());
    mFlag = static_cast<Qt::MatchFlag>(property("matchFlag", Qt::MatchStartsWith).toInt());
    if (mFlag == Qt::MatchContains){
        return value.toString().contains(property("value").toString(), mSensitivity);
    } else if (mFlag == Qt::MatchEndsWith){
        return QString::compare(value.toString().right(property("value").toString().length()), property("value").toString(), mSensitivity) == 0;
    }
    return QString::compare(value.toString().left(property("value").toString().length()), property("value").toString(), mSensitivity) == 0;
}

void N::TextFilter::setEditorData(QWidget * editor, const QModelIndex & index)
{
    TextFilterEditor* e = qobject_cast<TextFilterEditor*>(editor);
    if (e){
        QVariantMap p = index.data(Qt::EditRole).toMap();
        e->setText(p.value("value").toString());
        e->setMatchFlag(static_cast<Qt::MatchFlag>(p.value("matchFlag").toInt()));
        e->setCaseSensitivity(static_cast<Qt::CaseSensitivity>(p.value("caseSensitivity").toInt()));
    }
}

void N::TextFilter::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex & index)
{
    TextFilterEditor* e = qobject_cast<TextFilterEditor*>(editor);
    if (e){
        QVariantMap p(index.data(Qt::EditRole).toMap());
        p["value"] = e->text();
        p["caseSensitivity"] = e->caseSensitivity();
        p["matchFlag"] = e->matchFlag();
                if (property("enableOnCommit").toBool()){
                        p["enabled"] = true;
                }
        model->setData(index, p);
    }
}

void N::TextFilter::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem & option, const QModelIndex & index)
{
        editor->setGeometry(option.rect);
}

