#include "nAivPrivate.hpp"

N::ConditionalDecoration:: ConditionalDecoration       (void)
                         : AbstractItemModelDecoration (    )
{
  setProperty("dataRole", Qt::DisplayRole);
  setProperty("pixmapSize", QSize(16, 16));
  setProperty("column", -1);
}

N::ConditionalDecoration:: ConditionalDecoration       (int column)
                         : AbstractItemModelDecoration (          )
{
  setProperty("dataRole", Qt::DisplayRole);
  setProperty("column", column);
  setProperty("pixmapSize", QSize(16, 16));
}

N::ConditionalDecoration::~ConditionalDecoration (void)
{
}

void N::ConditionalDecoration::addCondition(N::ConditionalDecoration::MatchFlag matchFlag, const QVariant & value, const QString & set, const QString & name)
{
  QVariantList conditions  = property("conditions").toList();
  QVariantMap condition;
  condition["column"] = property("column").toInt();
  condition["matchFlag"] = matchFlag;
  condition["value"] = value;
  condition["set"] = set;
  condition["name"] = name;
  conditions.append(condition);
  setProperty("conditions", conditions);
}

N::ConditionalDecoration::MatchFlag N::ConditionalDecoration::matchFlag(int index) const
{
  if (index < property("conditions").toList().size()){
    return static_cast<N::ConditionalDecoration::MatchFlag>(property("conditions").toList().at(index).toMap().value("matchFlag").toInt());
  }
  return N::ConditionalDecoration::MatchFlagInvalid;
}

int N::ConditionalDecoration::count(void) const
{
  return property("conditions").toList().size();
}

QVariant N::ConditionalDecoration::decorate(const QModelIndex & index, int role) const
{
  const ConditionalDecorationProxyModel * model = qobject_cast<const ConditionalDecorationProxyModel*>(index.model()) ;
  if (model == 0) return QVariant();
  QVariantList definitions  = property("conditions").toList();
  QVariantMap m;
  QList<QPixmap> pl;
  for (int iDefinitions = 0; iDefinitions < definitions.size(); iDefinitions++) {
    m = definitions.at(iDefinitions).toMap();
    if (matches(index, m)){
      pl << model->icon(m.value("set").toString(), m.value("name").toString()).pixmap(model->iconSize());
      //return model->icon(p.value("set").toString(), p.value("name").toString()).pixmap(property("pixmapSize").toSize());
    }
  }
  if (!pl.isEmpty()) {
    QPixmap p(model->iconSize().width() * pl.size() + (pl.size() * model->iconSpacing()), model->iconSize().height());
    p.fill(Qt::transparent);
    QPainter painter(&p);
    for (int i = 0; i < pl.size(); i++){
      painter.drawPixmap(model->iconSize().width() * i + (i * model->iconSpacing()), 0, pl.at(i));
    }
    return p;
  } else {
    if (property("defaultSet").isNull()) {
      return QVariant();
    }
    return model->icon(property("defaultSet").toString(), property("defaultName").toString());
  }
}

QString N::ConditionalDecoration::iconName(int index) const
{
    if (index < property("conditions").toList().size()){
        return property("conditions").toList().at(index).toMap().value("name").toString();
    }
    return QString::null;
}

bool N::ConditionalDecoration::matches(const QModelIndex & index, const QVariantMap & properties) const
{
    QVariant data;
        data = index.model()->index(index.row(), properties.value("column", index.column()).toInt()).data(property("dataRole").toInt());
    N::ConditionalDecoration::MatchFlag matchFlag = static_cast<N::ConditionalDecoration::MatchFlag>(properties.value("matchFlag").toInt());
    if (matchFlag == N::ConditionalDecoration::Contains){
        if (data.toString().contains(properties.value("value").toString())){
            return true;
        }
    } else if (matchFlag == N::ConditionalDecoration::ContainsNot){
        if (!data.toString().contains(properties.value("value").toString())){
            return true;
        }
    } else if (matchFlag == N::ConditionalDecoration::EndsNotWith){
        if (!data.toString().endsWith(properties.value("value").toString())){
            return true;
        }
    } else if (matchFlag == N::ConditionalDecoration::EndsWith){
        if (data.toString().endsWith(properties.value("value").toString())){
            return true;
        }
    } else if (matchFlag == N::ConditionalDecoration::IsBetween){
        if (greaterOrEqualThan(data, properties.value("from")) && lessOrEqualThan(data, properties.value("to"))){
            return true;
        }
    } else if (matchFlag == N::ConditionalDecoration::IsEmpty){
        if (data.toString().isEmpty()){
            return true;
        }
    } else if (matchFlag == N::ConditionalDecoration::IsEqual){
        if (data == properties.value("value")){
            return true;
        }
    } else if(matchFlag == N::ConditionalDecoration::IsGreater){
        if (greaterThan(data, properties.value("value"))){
            return true;
        }
    } else if (matchFlag == N::ConditionalDecoration::IsGreaterOrEqual){
        if (greaterOrEqualThan(data, properties.value("value"))){
            return true;
        }
    } else if (matchFlag == N::ConditionalDecoration::IsLess){
        if (lessThan(data, properties.value("value"))){
            return true;
        }
    } else if(matchFlag == N::ConditionalDecoration::IsLessOrEqual){
        if (lessOrEqualThan(data, properties.value("value"))){
            return true;
        }
    } else if (matchFlag == N::ConditionalDecoration::IsNotBetween){
        if (lessThan(data, properties.value("from")) || greaterThan(data, properties.value("to"))){
            return true;
        }
    } else if (matchFlag == N::ConditionalDecoration::IsNotEmpty){
        if (!data.toString().isEmpty()){
            return true;
        }
    } else if (matchFlag == N::ConditionalDecoration::IsNotEqual){
        if (data != properties.value("value")){
            return true;
        }
    } else if (matchFlag == N::ConditionalDecoration::StartsNotWith){
        if (!data.toString().startsWith(properties.value("value").toString())){
            return true;
        }
    } else if (matchFlag == N::ConditionalDecoration::StartsWith){
        if (data.toString().startsWith(properties.value("value").toString())){
            return true;
        }
    }
    return false;
}

QVariantMap N::ConditionalDecoration::properties(int index) const
{
    if (index < property("conditions").toList().size()){
        return property("conditions").toList().at(index).toMap();
    }
    return QVariantMap();
}

void N::ConditionalDecoration::setHighlightRole(int role)
{
    setProperty("dataRole", role);
}

void N::ConditionalDecoration::setDefaultDecoration(const QString & set, const QString & name)
{
    setProperty("defaultSet", set);
    setProperty("defaultName", name);
}

QVariant N::ConditionalDecoration::value(int index) const
{
    if (index < property("conditions").toList().size()){
        return property("conditions").toList().at(index).toMap().value("value");
    }
    return QVariant();
}

bool N::ConditionalDecoration::equal(const QVariant &left, const QVariant &right) const
{
    if (left.type() == QVariant::Char){
        if (left.toChar() == right.toString().at(0)){
            return true;
        }
    } else if (left.type() == QVariant::Date){
        if (left.toDate() == right.toDate()){
            return true;
        }
    } else if (left.type() == QVariant::DateTime){
        if (left.toDateTime() == right.toDateTime()){
            return true;
        }
    } else if (left.type() == QVariant::Double){
        if (left.toDouble() == right.toDouble()){
            return true;
        }
    } else if (left.type() == QVariant::Int){
        if (left.toInt() == right.toInt()){
            return true;
        }
    } else if (left.type() == QVariant::LongLong){
        if (left.toLongLong() == right.toLongLong()){
            return true;
        }
    } else if (left.type() == QVariant::String){
        if (left.toString() == right.toString()){
            return true;
        }
    } else if (left.type() == QVariant::Time){
        if (left.toTime() == right.toTime()){
            return true;
        }
    } else if (left.type() == QVariant::UInt){
        if (left.toUInt() == right.toUInt()){
            return true;
        }
    } else if (left.type() == QVariant::ULongLong){
        if (left.toULongLong() == right.toULongLong()){
            return true;
        }
    }
    return false;
}

bool N::ConditionalDecoration::greaterOrEqualThan(const QVariant &left, const QVariant &right) const
{
    if (greaterThan(left, right)){
        return true;
    }
    if (equal(left, right)){
        return true;
    }
    return false;
}

bool N::ConditionalDecoration::greaterThan(const QVariant & left, const QVariant & right) const
{
    if (left.type() == QVariant::Char){
        if (left.toChar() > right.toString().at(0)){
            return true;
        }
    } else if (left.type() == QVariant::Date){
        if (left.toDate() > right.toDate()){
            return true;
        }
    } else if (left.type() == QVariant::DateTime){
        if (left.toDateTime() > right.toDateTime()){
            return true;
        }
    } else if (left.type() == QVariant::Double){
        if (left.toDouble() > right.toDouble()){
            return true;
        }
    } else if (left.type() == QVariant::Int){
        if (left.toInt() > right.toInt()){
            return true;
        }
    } else if (left.type() == QVariant::LongLong){
        if (left.toLongLong() > right.toLongLong()){
            return true;
        }
    } else if (left.type() == QVariant::String){
        if (left.toString() > right.toString()){
            return true;
        }
    } else if (left.type() == QVariant::Time){
        if (left.toTime() > right.toTime()){
            return true;
        }
    } else if (left.type() == QVariant::UInt){
        if (left.toUInt() > right.toUInt()){
            return true;
        }
    } else if (left.type() == QVariant::ULongLong){
        if (left.toULongLong() > right.toULongLong()){
            return true;
        }
    }
    return false;
}

bool N::ConditionalDecoration::lessOrEqualThan(const QVariant & left, const QVariant & right) const
{
    if (lessThan(left, right)){
        return true;
    }
    if (equal(left, right)){
        return true;
    }
    return false;
}

bool N::ConditionalDecoration::lessThan(const QVariant & left,const QVariant & right) const
{
    if (left.type() == QVariant::Char){
        if (left.toChar() < right.toString().at(0)){
            return true;
        }
    } else if (left.type() == QVariant::Date){
        if (left.toDate() < right.toDate()){
            return true;
        }
    } else if (left.type() == QVariant::DateTime){
        if (left.toDateTime() < right.toDateTime()){
            return true;
        }
    } else if (left.type() == QVariant::Double){
        if (left.toDouble() < right.toDouble()){
            return true;
        }
    } else if (left.type() == QVariant::Int){
        if (left.toInt() < right.toInt()){
            return true;
        }
    } else if (left.type() == QVariant::LongLong){
        if (left.toLongLong() < right.toLongLong()){
            return true;
        }
    } else if (left.type() == QVariant::String){
        if (left.toString() < right.toString()){
            return true;
        }
    } else if (left.type() == QVariant::Time){
        if (left.toTime() < right.toTime()){
            return true;
        }
    } else if (left.type() == QVariant::UInt){
        if (left.toUInt() < right.toUInt()){
            return true;
        }
    } else if (left.type() == QVariant::ULongLong){
        if (left.toULongLong() < right.toULongLong()){
            return true;
        }
    }
    return false;
}
