#ifndef PROTREEITEM_H
#define PROTREEITEM_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
class ProTreeItem : public QTreeWidgetItem
{
public:
    ProTreeItem(QTreeWidget* view, const QString &name, const QString &path ,int type = Type);
    ProTreeItem(QTreeWidgetItem *parent, const QString & name, const QString & path,
                QTreeWidgetItem* root, int type = Type);
    const QString GetPath();
    QTreeWidgetItem* GetRoot();
    void SetPreItem(QTreeWidgetItem *item);
    void SetNextItem(QTreeWidgetItem *item);
    ProTreeItem* GetPreItem();
    ProTreeItem* GetNextItem();
    ProTreeItem* GetLastPicChild();
    ProTreeItem* GetFirstPicChild();
private:
    QString path;
    QString name;
    QTreeWidgetItem *root;
    QTreeWidgetItem *preItem;
    QTreeWidgetItem *nextItem;
};

#endif // PROTREEITEM_H
