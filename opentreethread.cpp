#include "opentreethread.h"

#include <QDir>
#include <QtDebug>

#include "protreeitem.h"
#include "const.h"
OpenTreeThread::OpenTreeThread(const QString &srcPath, int fileCount,
                               QTreeWidget *self, QObject *parent)
    :QThread(parent),m_bStop(false),m_srcPath(srcPath),m_fileCount(fileCount),
      m_self(self),m_root(nullptr)
{

}

void OpenTreeThread::openProTree(const QString &srcPath, int &fileCount, QTreeWidget *self)
{
    QDir srcDir(srcPath);
    QString name = srcDir.dirName();
    ProTreeItem *item = new ProTreeItem(self,name,srcPath,TreeItemPro);
    item->setData(0,Qt::DisplayRole,name);
    item->setData(0,Qt::DecorationRole,QIcon(":/icon/dir.png"));
    item->setData(0,Qt::ToolTipRole,srcPath);
    m_root = item;
    recursiveProTree(srcPath,fileCount,self,m_root,item,nullptr);

}

void OpenTreeThread::run()
{
    openProTree(m_srcPath,m_fileCount,m_self);
    if(m_bStop&&m_root){
        auto path = dynamic_cast<ProTreeItem *>(m_root)->GetPath();
        auto index = m_self->indexOfTopLevelItem(m_root);
        delete m_self->takeTopLevelItem(index);
        QDir dir(path);
        dir.removeRecursively();
        return;
    }
    emit SigFinishProgress(m_fileCount);


}

void OpenTreeThread::recursiveProTree(const QString &srcPath, int &fileCount,
                                      QTreeWidget *self, QTreeWidgetItem *root,
                                      QTreeWidgetItem *parent, QTreeWidgetItem *preItem)
{
    QDir srcDir(srcPath);
    srcDir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    srcDir.setSorting(QDir::Name);
    QFileInfoList list = srcDir.entryInfoList();
    for(int i = 0; i< list.size(); i++){
        if(m_bStop){
            return;
        }
        QFileInfo fileInfo = list.at(i);
        bool bIsDir = fileInfo.isDir();
        if(bIsDir){
            if(m_bStop){
                return;
            }
            fileCount++;
            emit SigUpdateProgress(fileCount);
            ProTreeItem *item = new ProTreeItem(m_root,fileInfo.fileName()
                                                ,fileInfo.absoluteFilePath(),root,TreeItemDir);
            item->setData(0,Qt::DisplayRole,fileInfo.fileName());
            item->setData(0,Qt::DecorationRole,QIcon(":/icon/dir.png"));
            item->setData(0,Qt::ToolTipRole,fileInfo.absoluteFilePath());
            recursiveProTree(fileInfo.absoluteFilePath(),fileCount,self,m_root,item,preItem);
        }else{
            if(m_bStop){
                return;
            }
            const QString &suffix = fileInfo.completeSuffix();
            if(suffix != "png" && suffix != "jpeg" && suffix != "jpg"){
                continue;
            }
            fileCount ++;
            emit SigUpdateProgress(fileCount);
            ProTreeItem *item = new ProTreeItem(parent,fileInfo.fileName()
                                                ,fileInfo.absoluteFilePath(),root,TreeItemPic);
            item->setData(0,Qt::DisplayRole,fileInfo.fileName());
            item->setData(0,Qt::DecorationRole,QIcon(":/icon/pic.png"));
            item->setData(0,Qt::ToolTipRole,fileInfo.absoluteFilePath());

            if(preItem){
                ProTreeItem *preProItem = dynamic_cast<ProTreeItem *>(preItem);
                preProItem->SetNextItem(item);
            }
            item->SetPreItem(preItem);
            preItem = item;
        }
    }
    emit SigFinishProgress(fileCount);
}
