#include "protreeitem.h"

ProTreeItem::ProTreeItem(QTreeWidget* view, const QString &name, const QString &path ,int type)
    :QTreeWidgetItem(view,type),path(path),name(name),root(this),preItem(nullptr),nextItem(nullptr)
{

}
ProTreeItem::ProTreeItem(QTreeWidgetItem *parent, const QString & name, const QString & path,
            QTreeWidgetItem* root, int type)
    :QTreeWidgetItem(parent,type),path(path),name(name),root(root),preItem(nullptr),nextItem(nullptr)
{

}

const QString ProTreeItem::GetPath()
{
    return path;
}

QTreeWidgetItem *ProTreeItem::GetRoot()
{
    return root;
}

void ProTreeItem::SetPreItem(QTreeWidgetItem *item)
{
    preItem = item;
}

void ProTreeItem::SetNextItem(QTreeWidgetItem *item)
{
    nextItem = item;
}

ProTreeItem *ProTreeItem::GetPreItem()
{
    return dynamic_cast<ProTreeItem *>(preItem);
}

ProTreeItem *ProTreeItem::GetNextItem()
{
    return  dynamic_cast<ProTreeItem *>(nextItem);
}
