#include "protreeitem.h"
#include "const.h"
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

ProTreeItem *ProTreeItem::GetLastPicChild()
{
    if(this->type() == TreeItemPic)
        return nullptr;
    int childCount = this->childCount();
    if(childCount == 0)
        return nullptr;
    for(int i = childCount - 1; i>=0; i--){
        QTreeWidgetItem *lastchild =  this->child(i);
        ProTreeItem *lastTreeItem = dynamic_cast<ProTreeItem *>(lastchild);
        int itemType = lastTreeItem->type();
        if(itemType == TreeItemPic)
            return lastTreeItem;
        lastchild = lastTreeItem->GetLastPicChild();
        if(!lastchild){
            continue;
        }
        lastTreeItem = dynamic_cast<ProTreeItem *>(lastchild);
        return  lastTreeItem;
    }
    return  nullptr;
}

ProTreeItem *ProTreeItem::GetFirstPicChild()
{
    if(this->type() == TreeItemPic)
        return nullptr;
    int childCount = this->childCount();
    if(childCount == 0)
        return nullptr;
    for(int i = 0; i < childCount-1; i++){
        QTreeWidgetItem *firstChild =  this->child(i);
        ProTreeItem *firstTreeItem = dynamic_cast<ProTreeItem *>(firstChild);
        int itemType = firstTreeItem->type();
        if(itemType == TreeItemPic)
            return firstTreeItem;
        firstChild = firstTreeItem->GetFirstPicChild();
        if(!firstChild){
            continue;
        }
        firstTreeItem = dynamic_cast<ProTreeItem *>(firstChild);
        return  firstTreeItem;
    }
    return  nullptr;
}
