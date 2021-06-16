#include <qtaiv.h>

N::SelectionListFilter:: SelectionListFilter (int row, int column)
                       : AbstractFilter      (SelectionListFilter::Type, row, column)
{
  setProperty("dataSource", N::SelectionListFilter::Filter);
}

N::SelectionListFilter::~SelectionListFilter(void)
{
}

QWidget* N::SelectionListFilter::createEditor(FilterViewItemDelegate* delegate, QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
        SelectionListFilterEditor* e = new SelectionListFilterEditor(parent);
        QObject::connect(e, SIGNAL(cancelAndClose(QAbstractItemDelegate::EndEditHint)), delegate, SLOT(cancelAndClose(QAbstractItemDelegate::EndEditHint)));
        QObject::connect(e, SIGNAL(commitAndClose(QAbstractItemDelegate::EndEditHint)), delegate, SLOT(commitAndClose(QAbstractItemDelegate::EndEditHint)));
    return e;
}

QVariant N::SelectionListFilter::data(int role) const
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

N::SelectionListFilter::DataSource N::SelectionListFilter::dataSource() const
{
        return static_cast<N::SelectionListFilter::DataSource>(property("dataSource").toInt());
}

bool N::SelectionListFilter::matches(const QVariant & value, int type) const
{
	Q_UNUSED(type);
	if (property("mode").toInt() == 1){
		return value.toString().isEmpty();
	} else if (property("mode").toInt() == 2){
		return !value.toString().isEmpty();
	}
    return property("selectedValues").toList().contains(value);
}

void N::SelectionListFilter::setDataSource(N::SelectionListFilter::DataSource source)
{
        setProperty("dataSource", source);
}

void N::SelectionListFilter::setEditorData(QWidget* editor, const QModelIndex & index)
{
        Q_UNUSED(index);
    SelectionListFilterEditor* w = qobject_cast<SelectionListFilterEditor*>(editor);
    if (w){
                SelectionListFilterEditorPopup* p = qobject_cast<SelectionListFilterEditorPopup*>(w->popup());
                if (dataSource() == N::SelectionListFilter::Filter){
                        p->setValues(property("values").toList());
                } else if (dataSource() == N::SelectionListFilter::Model){
                        const FilterModel* m = qobject_cast<const FilterModel*>(index.model());
                        if (m){
                                p->setValues(m->sourceModel()->index(0, index.column()).data(AdvancedItemViews::SelectionListFilterDataRole).toList());
                        }
                }
        p->setSelectedValues(property("selectedValues").toList());
    }
}

void N::SelectionListFilter::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex & index)
{
        Q_UNUSED(index);
    SelectionListFilterEditor* w = qobject_cast<SelectionListFilterEditor*>(editor);
    if (w){
                SelectionListFilterEditorPopup* p = qobject_cast<SelectionListFilterEditorPopup*>(w->popup());
        QVariantMap properties(index.data(Qt::EditRole).toMap());
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

void N::SelectionListFilter::setValues(const QVariantList &values)
{
    setProperty("values", values);
}

void N::SelectionListFilter::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem & option, const QModelIndex & index)
{
	Q_UNUSED(index);
	SelectionListFilterEditor* e = qobject_cast<SelectionListFilterEditor*>(editor);
	if (e){
		e->setGeometry(option.rect);
		e->showPopup();
	}
}
