#ifndef PROTREETHEAD_H
#define PROTREETHEAD_H

#include <QThread>
#include <QTreeWidgetItem>
class ProTreeThead : public QThread
{
    Q_OBJECT
public:
    ProTreeThead(const QString &srcPath,const QString &distPath,QTreeWidgetItem *parentItem
                 ,int fileCount,QTreeWidget *self,QTreeWidgetItem *root,QObject *parent = nullptr);
    ~ProTreeThead();
protected:
    virtual void run();
signals:
    void SigUpdateProgress(int);
    void SigFinishProgress(int);

private:
    void CreateProTree(const QString &srcPath,const QString &distPath,QTreeWidgetItem *parentItem,
                       int fileCount,QTreeWidget *self,QTreeWidgetItem *root
                       ,QTreeWidgetItem *preItem = nullptr);
    QString srcPath;
    QString distPath;
    int fileCount;
    QTreeWidgetItem *parentItem;
    QTreeWidget *self;
    QTreeWidgetItem *root;
    bool bstop;
public slots:
    void SlotCancelProgress();
};

#endif // PROTREETHEAD_H
