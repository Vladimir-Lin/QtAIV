#include <qtaiv.h>

N::AutoFilter:: AutoFilter     (int row, int column)
              : AbstractFilter (AutoFilter::Type,row,column)
{
}

N::AutoFilter::~AutoFilter(void)
{
}

QWidget * N::AutoFilter::createEditor(FilterViewItemDelegate * delegate, QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
        AutoFilterEditor* e = new AutoFilterEditor(parent);
        QObject::connect(e, SIGNAL(cancelAndClose(QAbstractItemDelegate::EndEditHint)), delegate, SLOT(cancelAndClose(QAbstractItemDelegate::EndEditHint)));
        QObject::connect(e, SIGNAL(commitAndClose(QAbstractItemDelegate::EndEditHint)), delegate, SLOT(commitAndClose(QAbstractItemDelegate::EndEditHint)));
    return e;
}

QVariant N::AutoFilter::data(int role) const
{
    if (role == Qt::DisplayRole){
                if (property("mode").toInt() == 0){
                        if (property("selectedValues").toList().isEmpty()){
                                return QObject::tr("<none>");
                        } else {
                                if (property("selectedValues").toList().size() == 1){
                                        return QString(QObject::tr("%1 entry")).arg(property("selectedValues").toList().size());
                                } else {
                                        return QString(QObject::tr("%1 entries")).arg(property("selectedValues").toList().size());
                                }
                        }
                } else if (property("mode").toInt() == 1){
                        return QObject::tr("Empty");
                } else if (property("mode").toInt() == 2){
                        return QObject::tr("Not Empty");
                }
    }
    return QVariant();
}

bool N::AutoFilter::matches(const QVariant & value, int type) const
{
    Q_UNUSED(type);
        if (property("mode").toInt() == 1){
                return value.toString().isEmpty();
        } else if (property("mode").toInt() == 2){
                return !value.toString().isEmpty();
        }
    return property("selectedValues").toList().contains(value);
}

void N::AutoFilter::setEditorData(QWidget* editor, const QModelIndex & index)
{
    AutoFilterEditor* w = qobject_cast<AutoFilterEditor*>(editor);
    if (w){
      AutoFilterEditorPopup * p = qobject_cast<AutoFilterEditorPopup*>(w->popup());
      AbstractFilterModel   * m = qobject_cast<AbstractFilterModel*>((QAbstractItemModel*)index.model());
        if (m){
            p->setSourceModel(m->sourceModel(), column());
            p->setSelectedValues(property("selectedValues").toList());
        }
    }
}

void N::AutoFilter::setModelData(QWidget* editor, QAbstractItemModel * model, const QModelIndex & index)
{
    AutoFilterEditor* w = qobject_cast<AutoFilterEditor*>(editor);
    if (w){
        AutoFilterEditorPopup* p = qobject_cast<AutoFilterEditorPopup*>(w->popup());
        QVariantMap properties(index.data(Qt::EditRole).toMap());
        properties["selectedValues"] = p->selectedValues();
                properties["mode"] = p->mode();
                if (p->mode() > 0){
                        properties["selectedValues"] = QVariantList();
                } else {
                properties["selectedValues"] = p->selectedValues();
                }
                if (property("enableOnCommit").toBool()){
                        properties["enabled"] = true;
                }
        model->setData(index, properties);
    }
}

void N::AutoFilter::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem & option, const QModelIndex & index)
{
	Q_UNUSED(index);
	AutoFilterEditor* e = qobject_cast<AutoFilterEditor*>(editor);
	if (e){
		e->setGeometry(option.rect);
		e->showPopup();
	}
}
