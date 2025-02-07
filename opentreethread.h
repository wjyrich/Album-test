#ifndef OPENTREETHREAD_H
#define OPENTREETHREAD_H
#include <QTreeWidget>
#include <QThread>

class OpenTreeThread : public QThread
{
    Q_OBJECT
public:
    explicit OpenTreeThread(const QString& srcPath,int fileCount
                            ,QTreeWidget* self,QObject *parent = nullptr);
    void openProTree(const QString& srcPath,int& fileCount,QTreeWidget* self);
protected:
     void run() override;
private:
     void recursiveProTree(const QString& srcPath,int& fileCount
                           ,QTreeWidget* self,QTreeWidgetItem* root,QTreeWidgetItem* parent
                           ,QTreeWidgetItem* preItem);
     QString m_srcPath;
     int m_fileCount;
     QTreeWidget *m_self;
     bool m_bStop;
     QTreeWidgetItem *m_root;
signals:
     void SigFinishProgress(int);
     void SigUpdateProgress(int);
};

#endif // OPENTREETHREAD_H
