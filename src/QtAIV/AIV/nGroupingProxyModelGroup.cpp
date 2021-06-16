#include "nAivPrivate.hpp"

N::GroupingProxyModelGroup:: GroupingProxyModelGroup(GroupingProxyModelGroup * parent)
{
    cParent = parent;
    if (cParent){
        cParent->cChildren.append(this);
    }
    cSourceModelRow = -1;
}

N::GroupingProxyModelGroup:: GroupingProxyModelGroup(int sourceModelRow,GroupingProxyModelGroup * parent)
{
    cParent = parent;
    if (cParent){
        cParent->cChildren.append(this);
    }
    cSourceModelRow = sourceModelRow;
}

N::GroupingProxyModelGroup::~GroupingProxyModelGroup(void)
{
    qDeleteAll(cChildren);
}

void N::GroupingProxyModelGroup::addSourceModelRow(int row)
{
    if (!cSourceModelRows.contains(row)){
        cSourceModelRows.append(row);
    }
}

void N::GroupingProxyModelGroup::clear()
{
    for (int iChild = cChildren.size() - 1; iChild > -1; --iChild){
        if (cChildren.at(iChild)->sourceModelRow() == -1){
            cChildren.at(iChild)->clear();
        } else {
            delete cChildren.takeAt(iChild);
        }
    }
}

N::GroupingProxyModelGroup * N::GroupingProxyModelGroup::child(int index) const
{
    return cChildren.value(index);
}

int N::GroupingProxyModelGroup::childCount() const
{
    return cChildren.size();
}

QVariant N::GroupingProxyModelGroup::data(int role) const
{
    return cData.value(role);
}

N::GroupingProxyModelGroup * N::GroupingProxyModelGroup::findSourceModelRow(int sourceModelRow) const
{
    if (cSourceModelRow == sourceModelRow){
        return const_cast<GroupingProxyModelGroup*>(this);
    }
    GroupingProxyModelGroup* mGroup;
    Q_FOREACH(GroupingProxyModelGroup* mItem, cChildren){
        mGroup = mItem->findSourceModelRow(sourceModelRow);
        if (mGroup){
            return mGroup;
        }
    }
    return 0;
}

N::GroupingProxyModelGroup* N::GroupingProxyModelGroup::group(int sourceModelRow) const
{
    if (cSourceModelRow != -1 && cSourceModelRow == sourceModelRow){
        return parent();
    }
    GroupingProxyModelGroup* mGroup;
    Q_FOREACH(GroupingProxyModelGroup* mItem, cChildren){
        mGroup = mItem->findSourceModelRow(sourceModelRow);
        if (mGroup){
            return mGroup->parent();
        }
    }
    return 0;
}

int N::GroupingProxyModelGroup::indexOf(GroupingProxyModelGroup * group) const
{
    return cChildren.indexOf(group);
}

int N::GroupingProxyModelGroup::sourceModelRowIndexAt(int sourceModelRow) const
{
    for (int iChild = 0; iChild < cChildren.size(); iChild++){
        if (cChildren.at(iChild)->sourceModelRow() == sourceModelRow){
            return iChild;
        }
    }
    return -1;
}

N::GroupingProxyModelGroup* N::GroupingProxyModelGroup::matches(const QVariant & value) const
{
    if (cSourceModelRow != -1){
        return 0;
    }
    if (cParent != 0 && data(Qt::EditRole) == value){
        return const_cast<GroupingProxyModelGroup*>(this);
    }
    GroupingProxyModelGroup* mGroup;
    Q_FOREACH(GroupingProxyModelGroup* mItem, cChildren){
        mGroup = mItem->matches(value);
        if (mGroup){
            return mGroup;
        }
    }
    return 0;
}

void N::GroupingProxyModelGroup::moveSourceRow(int count)
{
    cSourceModelRow += count;
}

N::GroupingProxyModelGroup *N::GroupingProxyModelGroup::parent() const
{
    return cParent;
}

void N::GroupingProxyModelGroup::removeChild(int index)
{
    if (index < cChildren.size()){
        delete cChildren.takeAt(index);
    }
}

void N::GroupingProxyModelGroup::removeChildAtSourceModelRow(int sourceModelRow)
{
    for (int iChild = 0; iChild < cChildren.size(); iChild++){
        if (cChildren.at(iChild)->sourceModelRow() == sourceModelRow){
            delete cChildren.takeAt(iChild);
        }
    }
}

void N::GroupingProxyModelGroup::removeSourceModelRow(int row)
{
    cSourceModelRows.removeAt(cSourceModelRows.indexOf(row));
}

int N::GroupingProxyModelGroup::row() const
{
    if (cParent){
        return cParent->cChildren.indexOf(const_cast<GroupingProxyModelGroup*>(this));
    }
    return 0;
}

int N::GroupingProxyModelGroup::row(int sourceModelRow) const
{
    for (int iChild = 0; iChild < cChildren.size(); iChild++){
        if (cChildren.at(iChild)->sourceModelRow() == sourceModelRow){
            return iChild;
        }
    }
    return -1;
}

void N::GroupingProxyModelGroup::setData(const QVariant & data, int role)
{
    cData[role] = data;
}

int N::GroupingProxyModelGroup::sourceModelRow() const
{
    return cSourceModelRow;
}

CUIDs N::GroupingProxyModelGroup::sourceModelRows() const
{
    return cSourceModelRows;
}
