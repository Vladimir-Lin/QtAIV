#include <qtaiv.h>

N::ValueFilter:: ValueFilter    (int row, int column)
               : AbstractFilter (ValueFilter::Type, row, column)
{
    setProperty("caseSensitivity", Qt::CaseInsensitive);
}

N::ValueFilter::~ValueFilter()
{

}

QWidget* N::ValueFilter::createEditor(FilterViewItemDelegate* delegate, QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
        Q_UNUSED(delegate);
        Q_UNUSED(index);
    return new ValueFilterEditor(parent);
}

QVariant N::ValueFilter::data(int role) const
{
    if (role == Qt::DisplayRole){
        return QString("%1").arg(property("value").toString());
    }
    return QVariant();
}

bool N::ValueFilter::matches(const QVariant & value, int type) const
{
    Qt::CaseSensitivity s;
    Qt::MatchFlag mf;
    int t = type;
    if (t == -1){
        t = value.type();
    }
    switch(t)
    {
    case QVariant::Char:
        return value.toChar() == property("value").toChar();
    case QVariant::Date:
        return value.toDate() == property("value").toDate();
    case QVariant::DateTime:
        return value.toDateTime() == property("value").toDateTime();
    case QVariant::Double:
        return value.toDouble() == property("value").toDouble();
    case QVariant::Int:
        return value.toInt() == property("value").toInt();
    case QVariant::LongLong:
        return value.toLongLong() == property("value").toLongLong();
    case QVariant::String:
        s = static_cast<Qt::CaseSensitivity>(property("caseSensitivity", Qt::CaseInsensitive).toInt());
        mf = static_cast<Qt::MatchFlag>(property("matchFlag", Qt::MatchStartsWith).toInt());
        if (mf == Qt::MatchContains){
            return value.toString().contains(property("value").toString(), s);
        } else if (mf == Qt::MatchEndsWith){
            return QString::compare(value.toString().right(property("value").toString().length()), property("value").toString(), s) == 0;
        }
        return QString::compare(value.toString().left(property("value").toString().length()), property("value").toString(), s) == 0;
    case QVariant::Time:
        return value.toTime() == property("value").toTime();
    case QVariant::UInt:
        return value.toUInt() == property("value").toUInt();
    case QVariant::ULongLong:
        return value.toULongLong() == property("value").toULongLong();
    default:
        return value == property("value");
    }
    return false;
}

void N::ValueFilter::setEditorData(QWidget * editor, const QModelIndex & index)
{
    ValueFilterEditor* w = qobject_cast<ValueFilterEditor*>(editor);
    if (w){
        QVariantMap p = index.data(Qt::EditRole).toMap();
        w->setText(p.value("value").toString());
        w->setMatchFlag(static_cast<Qt::MatchFlag>(p.value("matchFlag").toInt()));
        w->setCaseSensitivity(static_cast<Qt::CaseSensitivity>(p.value("caseSensitivity").toInt()));
    }
}

void N::ValueFilter::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex & index)
{
    ValueFilterEditor* w = qobject_cast<ValueFilterEditor*>(editor);
    if (w){
        QVariantMap p(index.data(Qt::EditRole).toMap());
        p["value"] = w->text();
        p["caseSensitivity"] = w->caseSensitivity();
        p["matchFlag"] = w->matchFlag();
                if (property("enableOnCommit").toBool()){
                        p["enabled"] = true;
                }
        model->setData(index, p);
    }
}

void N::ValueFilter::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem & option, const QModelIndex & index)
{
        editor->setGeometry(option.rect);
}
