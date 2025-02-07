#include "protreethead.h"
#include <QDir>
#include <QtDebug>
#include "protreeitem.h"
#include "const.h"
ProTreeThead::ProTreeThead(const QString &srcPath,
                           const QString &distPath,
                           QTreeWidgetItem *parentItem,
                           int fileCount, QTreeWidget *self,
                           QTreeWidgetItem *root, QObject *parent)
            :QThread(parent),srcPath(srcPath),distPath(distPath),fileCount(fileCount)
            ,parentItem(parentItem),self(self),root(root),bstop(false)
{

}

ProTreeThead::~ProTreeThead()
{

}

void ProTreeThead::run()
{
    CreateProTree(srcPath,distPath,parentItem,fileCount,self,root);
    if(bstop){
        return;
    }
    emit SigFinishProgress(fileCount);
}

void ProTreeThead::CreateProTree(const QString &srcPath,
                                 const QString &distPath,
                                 QTreeWidgetItem *parentItem,
                                 int fileCount, QTreeWidget *self,
                                 QTreeWidgetItem *root, QTreeWidgetItem *preItem)
{
    if(bstop){
        return;
    }
    bool needCopy = true;
    if(srcPath == distPath){
        needCopy = false;
    }

    QDir importDir(srcPath);
    QStringList nameFilters;
    importDir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    importDir.setSorting(QDir::Name);
    QFileInfoList list = importDir.entryInfoList();
    for(int i = 0;i < list.size();i++){
        if(bstop){
            return;
        }
        QFileInfo fileInfo = list.at(i);
        bool bIsDir = fileInfo.isDir();
        if(bIsDir){
            //文件夹处理
            if(bstop)
                return;
            fileCount++;
            emit SigUpdateProgress(fileCount);
            QDir distDir(distPath);
            QString subDistPath = distDir.absoluteFilePath(fileInfo.fileName());
            QDir subDistDir(subDistPath);
            if(!subDistDir.exists()){
                bool ok = subDistDir.mkpath(subDistPath);
                if(!ok)
                    continue;
            }
            ProTreeItem *item = new ProTreeItem(parentItem,fileInfo.fileName(),subDistPath
                                                ,root,TreeItemDir);
            item->setData(0,Qt::DisplayRole,fileInfo.fileName());
            item->setData(0,Qt::DecorationRole,QIcon(":/icon/dir.png"));
            item->setData(0,Qt::ToolTipRole,subDistPath);
            qDebug()<<"item->setData(0,Qt::DecorationRole";
            CreateProTree(fileInfo.absoluteFilePath(),subDistPath,item,fileCount,self,root,preItem);

        }else{
            if(bstop)
                return;
            //取出完整文件后缀
            const QString &suffix = fileInfo.completeSuffix();
            if(suffix != "png" && suffix != "jpeg" && suffix != "jpg"){
                continue;
            }
            fileCount ++;
            emit SigUpdateProgress(fileCount);
            if(!needCopy){
                continue;
            }
            QDir distDir(distPath);
            QString distFilePath = distDir.absoluteFilePath(fileInfo.fileName());
            if(!QFile::copy(fileInfo.absoluteFilePath(),distFilePath)){
                continue;
            }
            ProTreeItem *item = new ProTreeItem(parentItem,fileInfo.fileName(),
                                                distPath,root,TreeItemPic);
            item->setData(0,Qt::DisplayRole,fileInfo.fileName());
            item->setData(0,Qt::DecorationRole,QIcon(":/icon/pic.png"));
            item->setData(0,Qt::ToolTipRole,distFilePath);
            if(preItem){
               ProTreeItem *preProItem = dynamic_cast<ProTreeItem *>(preItem);
               preProItem->SetNextItem(item);
            }
            item->SetPreItem(preItem);
            preItem = item;

        }
    }
}

void ProTreeThead::SlotCancelProgress()
{
    this->bstop = true;
}
