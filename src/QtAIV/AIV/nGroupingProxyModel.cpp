#include "nAivPrivate.hpp"

#define d ((GroupingProxyModelPrivate *)D)

N::GroupingProxyModel:: GroupingProxyModel(QObject * parent)
                      : QAbstractItemModel(parent)
                      , D((void *)new GroupingProxyModelPrivate(this))
{
    d->modelColumn = 0;
    d->groupItemDataRole = Qt::DisplayRole;

    d->sourceModel = new QStandardItemModel(this);
    d->root = new GroupingProxyModelGroup();
    GroupingProxyModelGroup* item = new GroupingProxyModelGroup(d->root);
    item->setData(tr("Ungrouped"), Qt::DisplayRole);
}

N::GroupingProxyModel::~GroupingProxyModel(void)
{
  delete d ;
  D = NULL ;
}

void N::GroupingProxyModel::addGroup(const QString & text, const QVariant & value)
{
    GroupingProxyModelGroup * g = new GroupingProxyModelGroup(d->root);
    g->setData(text, Qt::DisplayRole);
    if (value.isNull()){
        g->setData(text, Qt::EditRole);
    } else {
        g->setData(value, Qt::EditRole);
    }
    buildGroups();
}

void N::GroupingProxyModel::addGroup(const QIcon & icon, const QString &text, const QVariant &value)
{
    GroupingProxyModelGroup * g = new GroupingProxyModelGroup(d->root);
    g->setData(icon, Qt::DecorationRole);
    g->setData(text, Qt::DisplayRole);
    if (value.isNull()){
        g->setData(text, Qt::EditRole);
    } else {
        g->setData(value, Qt::EditRole);
    }
    buildGroups();
}

int N::GroupingProxyModel::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return d->sourceModel->columnCount() + 1;
}

QVariant N::GroupingProxyModel::data(const QModelIndex & proxyIndex, int role) const
{
    if (!proxyIndex.isValid()){
        return QVariant();
    }
//    if (role == Qt::DisplayRole){
//        if (proxyIndex.column() == 0){
//            QGroupingProxyModelGroup* mItem = static_cast<QGroupingProxyModelGroup*>(proxyIndex.internalPointer());
//            if (mItem->parent() == d->root){
//                return QString("%1 [%2]").arg(mItem->data(Qt::DisplayRole).toString()).arg(mItem->childCount());
//            } else {
//                return "A";
//            }
//        } else {

//        }
//    }
    GroupingProxyModelGroup* item = static_cast<GroupingProxyModelGroup*>(proxyIndex.internalPointer());
    if (item->parent() == d->root){
        if (proxyIndex.column() == 0){
            if (role == Qt::DisplayRole){
                return QString("%1 [%2]").arg(item->data(Qt::DisplayRole).toString()).arg(item->childCount());
            } else {
                return item->data(role);
            }
        }
    } else {
        if (proxyIndex.column() == 0){
            if (role == Qt::DisplayRole){
                return "";
            }
        }
    }
    return mapToSource(proxyIndex).data(role);
}

Qt::ItemFlags N::GroupingProxyModel::flags(const QModelIndex & index) const
{
    GroupingProxyModelGroup * item = static_cast<GroupingProxyModelGroup*>(index.internalPointer());
    if (item && item->parent() == d->root){
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    } else {
        if (index.column() == 0){
            return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        }
    }
    return d->sourceModel->flags(mapToSource(index));
}

QVariant N::GroupingProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section == 0){
        if (role == Qt::DisplayRole){
            return tr("Group");
        }
        return QVariant();
    }
    return d->sourceModel->headerData(section - 1, orientation, role);
}

QModelIndex N::GroupingProxyModel::index(int row, int column, const QModelIndex & parent) const
{
    if (!hasIndex(row, column, parent)){
        return QModelIndex();
    }
    GroupingProxyModelGroup* mParentItem;
    GroupingProxyModelGroup* mChildItem;
    if (!parent.isValid()){
        mParentItem = d->root;
    } else {
        mParentItem = static_cast<GroupingProxyModelGroup*>(parent.internalPointer());
    }
    mChildItem = mParentItem->child(row);
    if (mChildItem){
        return createIndex(row, column, mChildItem);
    }
    return QModelIndex();
}

int N::GroupingProxyModel::findText(const QString & text) const
{
    for (int iGroup = 0; iGroup < d->root->childCount(); iGroup++){
        if (d->root->child(iGroup)->data(Qt::DisplayRole).toString() == text){
            return iGroup;
        }
    }
    return -1;
}

QModelIndex N::GroupingProxyModel::mapFromSource(const QModelIndex & sourceIndex) const
{
    return QModelIndex();
}

QModelIndex N::GroupingProxyModel::mapToSource(const QModelIndex & proxyIndex) const
{
    GroupingProxyModelGroup* item = static_cast<GroupingProxyModelGroup*>(proxyIndex.internalPointer());
    if (item == d->root){
        return QModelIndex();
    }
    if (item == 0){
        return QModelIndex();
    }
    return d->sourceModel->index(item->sourceModelRow(), proxyIndex.column() - 1);
}

int N::GroupingProxyModel::modelColumn() const
{
    return d->modelColumn;
}

QModelIndex N::GroupingProxyModel::parent(const QModelIndex & child) const
{
    if (!child.isValid()){
        return QModelIndex();
    }
    GroupingProxyModelGroup* childItem = static_cast<GroupingProxyModelGroup*>(child.internalPointer());
    GroupingProxyModelGroup* parentItem = childItem->parent();
    if (parentItem == d->root){
        return QModelIndex();
    }
    return createIndex(parentItem->row(), 0, parentItem);
}

bool N::GroupingProxyModel::removeGroup(int index)
{
    if (index < 1){
        return false;
    }
    beginRemoveRows(QModelIndex(), index, index);
    d->root->removeChild(index);
    buildGroups();
    endRemoveRows();
    return true;
}

bool N::GroupingProxyModel::restoreGroups(const QByteArray & data)
{
    QByteArray groups(data);
    QDataStream stream(&groups, QIODevice::ReadOnly);
    qint32 version;

    stream >> version;
    if (version == 1){
        beginResetModel();

        for (int iGroup = 1; iGroup < d->root->childCount(); iGroup++){
            d->root->removeChild(iGroup);
        }
        QIcon icon;
        QString text;
        QVariant value;
        stream >> d->modelColumn >> d->groupItemDataRole;
        while(!stream.atEnd()){
            stream >> icon >> text >> value;
            addGroup(icon, text, value);
        }
        endResetModel();
        buildGroups();
        return true;
    }
    return false;
}

int N::GroupingProxyModel::rowCount(const QModelIndex & parent) const
{
    if (parent.column() > 0){
        return 0;
    }
    GroupingProxyModelGroup* parentItem;
    if (!parent.isValid()){
        parentItem = d->root;
    } else {
        parentItem = static_cast<GroupingProxyModelGroup*>(parent.internalPointer());
    }
    return parentItem->childCount();
}

bool N::GroupingProxyModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (!index.parent().isValid()){
        d->root->child(index.row())->setData(value, role);
    }
    return d->sourceModel->setData(mapToSource(index), value, role);
}

QByteArray N::GroupingProxyModel::saveGroups() const
{
    QByteArray groups;
    QDataStream stream(&groups, QIODevice::WriteOnly);
    stream << qint32(1) << qint32(d->modelColumn) << qint32(d->groupItemDataRole);

    for (int iGroup = 1; iGroup < d->root->childCount(); iGroup++){
        stream << d->root->child(iGroup)->data(Qt::DecorationRole)
                << d->root->child(iGroup)->data(Qt::DisplayRole)
                << d->root->child(iGroup)->data(Qt::EditRole);
    }
    return groups;
}

void N::GroupingProxyModel::setModelColumn(int column)
{
    d->modelColumn = column;
    // @todo filter
}

void N::GroupingProxyModel::setSourceModel(QAbstractItemModel* sourceModel)
{
    d->sourceModel = sourceModel;
    buildGroups();
    connect(d->sourceModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(dataChangedHandler(QModelIndex,QModelIndex)));
    connect(d->sourceModel, SIGNAL(rowsAboutToBeInserted(QModelIndex, int, int)), this, SLOT(rowsAboutToBeInsertedHandler(QModelIndex,int,int)));
    connect(d->sourceModel, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(rowsInsertedHandler(QModelIndex,int,int)));
}

void N::GroupingProxyModel::dataChangedHandler(const QModelIndex & topLeft, const QModelIndex & bottomRight)
{
    if (topLeft.column() != d->modelColumn){
        GroupingProxyModelGroup* mCurrentGroup = d->root->group(topLeft.row());
        QModelIndex mTopLeft = index(mCurrentGroup->parent()->indexOf(mCurrentGroup), 0);
        QModelIndex mBottomRight = index(mTopLeft.row(), d->sourceModel->columnCount());
        emit dataChanged(mTopLeft, mBottomRight);
    } else {
        GroupingProxyModelGroup* currentGroup = d->root->group(topLeft.row());
        GroupingProxyModelGroup* newGroup = d->root->matches(d->sourceModel->index(topLeft.row(), d->modelColumn).data(d->groupItemDataRole));
        if (newGroup == 0){
            newGroup = d->root->child(0);
        }
        if (currentGroup != newGroup){
            QModelIndex p;
            p = index(currentGroup->parent()->indexOf(currentGroup), 0);
            beginRemoveRows(p, currentGroup->row(topLeft.row()), currentGroup->row(topLeft.row()));
            currentGroup->removeChildAtSourceModelRow(topLeft.row());
            endRemoveRows();

            p = index(newGroup->parent()->indexOf(newGroup), 0);
            beginInsertRows(p, newGroup->childCount(), newGroup->childCount());
            (void)new GroupingProxyModelGroup(topLeft.row(), newGroup);
            endInsertRows();
        }
    }
}

void N::GroupingProxyModel::rowsAboutToBeInsertedHandler(const QModelIndex & parent, int start, int end)
{
//    d->root->child(0)->addSourceModelRow(start);
    //    qDebug() << d->root->child(0)->sourceModelRows();
}

void N::GroupingProxyModel::rowsInsertedHandler(const QModelIndex & parent, int first, int last)
{
    // @todo
//    int mGroup = d->root->group(cSourceModel->index(first, cModelColumn).data(cGroupItemDataRole));
//    beginResetModel();
//    moveRows(first, 1);
//    if (mGroup == -1){
//        new QGroupingProxyModelGroup(first, d->root->child(0));
//    } else {
//        new QGroupingProxyModelGroup(first, d->root->child(mGroup));
//    }
    //    endResetModel();
}

void N::GroupingProxyModel::buildGroups()
{
    beginResetModel();
    d->root->clear();
    if (d->sourceModel){
        GroupingProxyModelGroup* group;
        for (int iRow = 0; iRow < d->sourceModel->rowCount(); iRow++){
            group = d->root->matches(d->sourceModel->index(iRow, d->modelColumn).data(d->groupItemDataRole));
            if (group == 0){
                group = d->root->child(0);
            }
            new GroupingProxyModelGroup(iRow, group);
        }
    }
    endResetModel();
}

int N::GroupingProxyModel::groupAt(int sourceModelRow)
{
    for (int iGroup = 0; iGroup < d->root->childCount(); iGroup++){
        for (int iRow = 0; iRow < d->root->child(iGroup)->childCount(); iRow++){
            if (d->root->child(iGroup)->child(iRow)->sourceModelRow() == sourceModelRow){
                return iGroup;
            }
        }
    }
    return -1;
}

void N::GroupingProxyModel::moveRows(int row, int count)
{
    for (int iGroup = 0; iGroup < d->root->childCount(); iGroup++){
        for (int iChild = 0; iChild < d->root->child(iGroup)->childCount(); iChild++){
            if (d->root->child(iGroup)->child(iChild)->sourceModelRow() >= row){
                d->root->child(iGroup)->child(iChild)->moveSourceRow(count);
            }
        }
    }
}

void N::GroupingProxyModel::removeSourceModelRow(int sourceModelRow)
{
    for (int iGroup = 0; iGroup < d->root->childCount(); iGroup++){
        d->root->child(iGroup)->removeChildAtSourceModelRow(sourceModelRow);
    }
}
