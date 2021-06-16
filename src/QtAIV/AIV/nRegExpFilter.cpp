#include <qtaiv.h>

N::RegExpFilter:: RegExpFilter   (int row, int column)
                : AbstractFilter (N::RegExpFilter::Type, row, column)
{
    setProperty("caseSensitivity", Qt::CaseInsensitive);
    // @todo change default for Qt5
    setProperty("patternSyntax", QRegExp::RegExp);
}

N::RegExpFilter::~RegExpFilter(void)
{
}

QWidget* N::RegExpFilter::createEditor(FilterViewItemDelegate* delegate, QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    return new RegExpFilterEditor(parent);
}

QVariant N::RegExpFilter::data(int role) const
{
    if (role == Qt::DisplayRole){
        return QString("%1").arg(property("pattern").toString());
    }
    return QVariant();
}

bool N::RegExpFilter::matches(const QVariant & value, int type) const
{
    Q_UNUSED(type);
    QRegExp regExp(property("pattern").toString(), static_cast<Qt::CaseSensitivity>(property("caseSensitivity").toInt()), static_cast<QRegExp::PatternSyntax>(property("patternSyntax").toInt()));
    if (regExp.indexIn(value.toString()) == -1){
        return false;
    }
    return true;
}

void N::RegExpFilter::setEditorData(QWidget * editor, const QModelIndex & index)
{
    RegExpFilterEditor* w = qobject_cast<RegExpFilterEditor*>(editor);
    if (w){
        w->setCaseSensitivity(static_cast<Qt::CaseSensitivity>(property("caseSensitivity").toInt()));
        w->setPattern(property("pattern").toString());
        w->setPatternSyntax(static_cast<QRegExp::PatternSyntax>(property("patternSyntax").toInt()));
    }
}

void N::RegExpFilter::setModelData(QWidget* editor, QAbstractItemModel * model, const QModelIndex & index)
{
    RegExpFilterEditor* w = qobject_cast<RegExpFilterEditor*>(editor);
    if (w){
        QVariantMap p(index.data(Qt::EditRole).toMap());
        p["pattern"] = w->pattern();
        p["caseSensitivity"] = w->caseSenstivity();
        p["patternSyntax"] = w->patternSyntax();
                if (property("enableOnCommit").toBool()){
                        p["enabled"] = true;
                }
        model->setData(index, p);
    }
}

void N::RegExpFilter::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem & option, const QModelIndex & index)
{
        editor->setGeometry(option.rect);
}
