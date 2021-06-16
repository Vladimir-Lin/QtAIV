#include <qtaiv.h>

N::FilterModel:: FilterModel         (QObject * parent)
               : AbstractFilterModel (          parent)
{
    registerFilter(TextFilter::Type, QIcon(":/qaiv/filter/text"), tr("Text Filter"), tr("Filter on the textual representation of values"));
    registerFilter(RangeFilter::Type, QIcon(":/qaiv/filter/range"), tr("Range Filter"));
    registerFilter(ValueFilter::Type, QIcon(":/qaiv/filter/value"), tr("Value Filter"));
    registerFilter(RegExpFilter::Type, QIcon(":/qaiv/filter/regexp"), tr("RegExp Filter"));
    registerFilter(AutoFilter::Type, QIcon(":/qaiv/filter/auto"), tr("Auto Filter"));
    registerFilter(SelectionListFilter::Type, QIcon(":/qaiv/filter/selection"), tr("Selection List Filter"));
}

N::FilterModel::~FilterModel(void)
{
}

N::AbstractFilter* N::FilterModel::createFilter(const QModelIndex & index, const QVariantMap & properties) const
{
    AbstractFilter* filter = 0;
    FilterGroup* group = filterGroup(index);
    if (group == 0){
        return 0;
    }
    if (index.row() < rowCount()){
        if (properties.value("type").toInt() == RangeFilter::Type){
            filter = new RangeFilter(index.row(), index.column());
        } else if (properties.value("type").toInt() == ValueFilter::Type){
            filter = new ValueFilter(index.row(), index.column());
        } else if (properties.value("type").toInt() == TextFilter::Type){
            filter = new TextFilter(index.row(), index.column());
        } else if (properties.value("type").toInt() == AutoFilter::Type){
            filter = new AutoFilter(index.row(), index.column());
        } else if (properties.value("type").toInt() == RegExpFilter::Type){
            filter = new RegExpFilter(index.row(), index.column());
        } else if (properties.value("type").toInt() == SelectionListFilter::Type){
            filter = new SelectionListFilter(index.row(), index.column());
        }
    }
    if (filter){
                filter->setProperty("enableOnCommit", true);
        group->add(filter);
    }
    return filter;
}
