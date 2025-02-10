#ifndef PRELISTITEM_H
#define PRELISTITEM_H

#include <QListWidgetItem>

class PreListItem : public QListWidgetItem
{
public:
    PreListItem(const QIcon &icon,const QString &text,const int &index
                ,QListWidget *listview = nullptr, int type = Type);
    int GetIndex();
    QString GetPath();
private:
    QString m_path;
    int m_index;
};

#endif // PRELISTITEM_H
