#include <qtaiv.h>

N::RangeFilter:: RangeFilter    (int row, int column)
               : AbstractFilter (RangeFilter::Type, row, column)
{
}

N::RangeFilter::~RangeFilter(void)
{
}

QWidget * N::RangeFilter::createEditor(FilterViewItemDelegate* delegate, QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
        RangeFilterEditor* e = new RangeFilterEditor(parent);
        QObject::connect(e, SIGNAL(commitAndClose(QAbstractItemDelegate::EndEditHint)), delegate, SLOT(commitAndClose(QAbstractItemDelegate::EndEditHint)));
    return e;
}

QVariant N::RangeFilter::data(int role) const
{
    if (role == Qt::DisplayRole){
                if (property("rangeFrom").toString().isNull() && property("rangeTo").toString().isNull()){
                        return QObject::tr("<any> - <any>");
                } else if (property("rangeFrom").toString().isNull() && !property("rangeTo").toString().isNull()){
                return QString(QObject::tr("<any> - %1")).arg(property("rangeTo").toString());
                } else if (!property("rangeFrom").toString().isNull() && property("rangeTo").toString().isNull()){
                return QString(QObject::tr("%1 - <any>")).arg(property("rangeFrom").toString());
                }
        return QString("%1 - %2").arg(property("rangeFrom").toString()).arg(property("rangeTo").toString());
    }
    return QVariant();
}

bool N::RangeFilter::matches(const QVariant & value, int type) const
{
        Q_UNUSED(type);
    if (value.type() == QVariant::Char){
        if (property("rangeFrom").isValid() && property("rangeTo").isValid()){
            return value.toChar() >= property("rangeFrom").toString().at(0) && value.toChar() <= property("rangeTo").toString().at(0);
        } else if (property("rangeFrom").isValid()){
            return value.toChar() >= property("rangeFrom").toString().at(0);
        } else if (property("rangeTo").isValid()){
            return value.toChar() <= property("rangeTo").toString().at(0);
        }
    } else if (value.type() == QVariant::Date){
        if (property("rangeFrom").isValid() && property("rangeTo").isValid()){
            return value.toDate() >= property("rangeFrom").toDate() && value.toDate() <= property("rangeTo").toDate();
        } else if (property("rangeFrom").isValid()){
            return value.toDate() >= property("rangeFrom").toDate();
        } else if (property("rangeTo").isValid()){
            return value.toDate() <= property("rangeTo").toDate();
        }
    } else if (value.type() == QVariant::DateTime){
        if (property("rangeFrom").isValid() && property("rangeTo").isValid()){
            return value.toDateTime() >= property("rangeFrom").toDateTime() && value.toDateTime() <= property("rangeTo").toDateTime();
        } else if (property("rangeFrom").isValid()){
            return value.toDateTime() >= property("rangeFrom").toDateTime();
        } else if (property("rangeTo").isValid()){
            return value.toDateTime() <= property("rangeTo").toDateTime();
        }
    } else if (value.type() == QVariant::Double){
        if (property("rangeFrom").isValid() && property("rangeTo").isValid()){
            return value.toDouble() >= property("rangeFrom").toDouble() && value.toDouble() <= property("rangeTo").toDouble();
        } else if (property("rangeFrom").isValid()){
            return value.toDouble() >= property("rangeFrom").toDouble();
        } else if (property("rangeTo").isValid()){
            return value.toDouble() <= property("rangeTo").toDouble();
        }
    } else if (value.type() == QVariant::Int){
        if (property("rangeFrom").isValid() && property("rangeTo").isValid()){
            return value.toInt() >= property("rangeFrom").toInt() && value.toInt() <= property("rangeTo").toInt();
        } else if (property("rangeFrom").isValid()){
            return value.toInt() >= property("rangeFrom").toInt();
        } else if (property("rangeTo").isValid()){
            return value.toInt() <= property("rangeTo").toInt();
        }
    } else if (value.type() == QVariant::LongLong){
        if (property("rangeFrom").isValid() && property("rangeTo").isValid()){
            return value.toLongLong() >= property("rangeFrom").toLongLong() && value.toLongLong() <= property("rangeTo").toLongLong();
        } else if (property("rangeFrom").isValid()){
            return value.toLongLong() >= property("rangeFrom").toLongLong();
        } else if (property("rangeTo").isValid()){
            return value.toLongLong() <= property("rangeTo").toLongLong();
        }
    } else if (value.type() == QVariant::String){
        if (property("rangeFrom").isValid() && property("rangeTo").isValid()){
            return value.toString() >= property("rangeFrom").toString() && value.toString() <= property("rangeTo").toString();
        } else if (property("rangeFrom").isValid()){
            return value.toString() >= property("rangeFrom").toString();
        } else if (property("rangeTo").isValid()){
            return value.toString() <= property("rangeTo").toString();
        }
    } else if (value.type() == QVariant::Time){
        if (property("rangeFrom").isValid() && property("rangeTo").isValid()){
            return value.toTime() >= property("rangeFrom").toTime() && value.toTime() <= property("rangeTo").toTime();
        } else if (property("rangeFrom").isValid()){
            return value.toTime() >= property("rangeFrom").toTime();
        } else if (property("rangeTo").isValid()){
            return value.toTime() <= property("rangeTo").toTime();
        }
    } else if (value.type() == QVariant::UInt){
        if (property("rangeFrom").isValid() && property("rangeTo").isValid()){
            return value.toUInt() >= property("rangeFrom").toUInt() && value.toUInt() <= property("rangeTo").toUInt();
        } else if (property("rangeFrom").isValid()){
            return value.toUInt() >= property("rangeFrom").toUInt();
        } else if (property("rangeTo").isValid()){
            return value.toUInt() <= property("rangeTo").toUInt();
        }
    } else if (value.type() == QVariant::ULongLong){
        if (property("rangeFrom").isValid() && property("rangeTo").isValid()){
            return value.toULongLong() >= property("rangeFrom").toULongLong() && value.toULongLong() <= property("rangeTo").toULongLong();
        } else if (property("rangeFrom").isValid()){
            return value.toULongLong() >= property("rangeFrom").toULongLong();
        } else if (property("rangeTo").isValid()){
            return value.toULongLong() <= property("rangeTo").toULongLong();
        }
    }
    return true;
}

void N::RangeFilter::setEditorData(QWidget * editor, const QModelIndex & index)
{
        Q_UNUSED(index);
    RangeFilterEditor* w = qobject_cast<RangeFilterEditor*>(editor);
    if (w){
      RangeFilterEditorPopup* p = qobject_cast<RangeFilterEditorPopup*>(w->popup());
      p->setRangeFrom(property("rangeFrom").toString());
      p->setRangeTo(property("rangeTo").toString());
    }
}

void N::RangeFilter::setModelData(QWidget* editor, QAbstractItemModel * model, const QModelIndex & index)
{
        Q_UNUSED(index);
    RangeFilterEditor* w = qobject_cast<RangeFilterEditor*>(editor);
    if (w){
                RangeFilterEditorPopup* p = qobject_cast<RangeFilterEditorPopup*>(w->popup());
        QVariantMap properties(index.data(Qt::EditRole).toMap());
        if (p->rangeFrom().isEmpty()){
            properties["rangeFrom"] = QVariant();
        } else {
            properties["rangeFrom"] = p->rangeFrom();
        }
        if (p->rangeTo().isEmpty()){
            properties["rangeTo"] =  QVariant();
        } else {
            properties["rangeTo"] = p->rangeTo();
        }
        model->setData(index, properties);
    }
}

void N::RangeFilter::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem & option, const QModelIndex & index)
{
	RangeFilterEditor* e = qobject_cast<RangeFilterEditor*>(editor);
	e->setGeometry(option.rect);
	e->showPopup();
}
