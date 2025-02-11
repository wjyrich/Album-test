#ifndef PRILISTWID_H
#define PRILISTWID_H
#include <QListWidget>
#include <QTreeWidgetItem>

class PriListWid : public QListWidget
{
    Q_OBJECT
public:
    PriListWid(QWidget *parent = nullptr);
    ~PriListWid();
private:
    QMap<QString,QListWidgetItem *> m_setItems;
    void AddListItem(const QString &path);
    int m_global;
    QPoint m_posOrigin;
    int m_lastIndex;
public slots:
    void SlotUpPreList(QTreeWidgetItem * treeItem);
    void SlotSelectItem(QTreeWidgetItem *treeItem);
    void SlotItemPressed(QListWidgetItem *item);
signals:
    void SigupSelectShow(QString path);
};

#endif // PRILISTWID_H
