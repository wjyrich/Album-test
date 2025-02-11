#include "protreewidget.h"
#include <QDir>
#include <QHeaderView>
#include <QGuiApplication>
#include <QFileDialog>
#include <QMenu>
#include <QtDebug>

#include "protreeitem.h"
#include "const.h"
#include "removeprodialog.h"
#include "slideshowdlg.h"
ProTreeWidget::ProTreeWidget(QWidget *parent):QTreeWidget (parent)
  ,rightButtonItem(nullptr),activeItem(nullptr),selectItem(nullptr),dialogProgress(nullptr)
  ,dialogProgressOpen(nullptr),threadCreatePro(nullptr),m_openTreeThread(nullptr)
{
    qRegisterMetaType<QVector<int> >("QVector<int>");
    this->header()->hide();
    connect(this,&ProTreeWidget::itemPressed,this,&ProTreeWidget::SlotItemPressed);
    actionImport = new QAction(QIcon(":/icon/import.png"),tr("导入文件"),this);
    actionSetstart = new QAction(QIcon(":/icon/core.png"),tr("设置活动项目"),this);
    actionClosePro = new QAction(QIcon(":/icon/close.png"),tr("关闭项目"),this);
    actionSlideShow = new QAction(QIcon(":/icon/slideshow.png"),tr("轮询播放"),this);
    connect(actionImport,&QAction::triggered,this,&ProTreeWidget::SlotImport);
    connect(actionSetstart,&QAction::triggered,this,&ProTreeWidget::SlotSetStart);
    connect(actionClosePro,&QAction::triggered,this,&ProTreeWidget::SlotClosePro);
    connect(actionSlideShow,&QAction::triggered,this,&ProTreeWidget::SlotSlideShow);
    connect(this,&ProTreeWidget::itemDoubleClicked,this,&ProTreeWidget::SlotDoubleClickedItem);

    m_player = new QMediaPlayer();
    m_playList = new QMediaPlaylist();
    m_playList->setPlaybackMode(QMediaPlaylist::Loop);
    m_player->setPlaylist(m_playList);
}

void ProTreeWidget::addProToTree(const QString &name, const QString &path)
{
    QDir dir(path);
    QString filePath = dir.absoluteFilePath(name);
    if(setPath.find(filePath) != setPath.end()){
        return;
    }
    QDir proDir(filePath);
    if(!proDir.exists()){
        bool enable = proDir.mkpath(filePath);
        if(!enable){
            return;
        }
    }
    setPath.insert(filePath);
    ProTreeItem *item = new ProTreeItem(this,name,filePath,TreeItemPro);
    item->setData(0,Qt::DisplayRole,name);
    item->setData(0,Qt::DecorationRole,QIcon(":/icon/dir.png"));
    item->setData(0,Qt::ToolTipRole,filePath);
    this->addTopLevelItem(item);
}

void ProTreeWidget::SlotItemPressed(QTreeWidgetItem *pressItem, int column)
{
    if(QGuiApplication::mouseButtons() == Qt::RightButton){
        QMenu menu(this);
        int itemType = pressItem->type();
        if(itemType == TreeItemPro){
            rightButtonItem = pressItem;
            menu.addAction(actionImport);
            menu.addAction(actionSetstart);
            menu.addAction(actionClosePro);
            menu.addAction(actionSlideShow);
            //右键后能够出现相应的菜单在鼠标右键点击时位置
            menu.exec(QCursor::pos());

        }
    }
}

void ProTreeWidget::SlotDoubleClickedItem(QTreeWidgetItem *doubleClickedItem, int col)
{
    if(QGuiApplication::mouseButtons() == Qt::LeftButton){
        ProTreeItem *treeDobuleItem = dynamic_cast<ProTreeItem *>(doubleClickedItem);
        if(!treeDobuleItem){
            return;
        }
        int itemType = treeDobuleItem->type();
        if(itemType == TreeItemPic){
            emit SigUpdateSelected(treeDobuleItem->GetPath());
            selectItem = treeDobuleItem;
        }
    }
}

void ProTreeWidget::SlotImport()
{
    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::Directory);
    fileDialog.setWindowTitle("选择导入文件夹");
    QString path = "";
    if(!rightButtonItem){
        qDebug()<<"empty!";
        return;
    }
    path = dynamic_cast<ProTreeItem *>(rightButtonItem)->GetPath();
    fileDialog.setDirectory(path);
    fileDialog.setViewMode(QFileDialog::Detail);

    QStringList fileName;
    if(fileDialog.exec()){
        fileName = fileDialog.selectedFiles();
    }
    if(fileName.length()<0){
        return;
    }
    QString importPath = fileName.at(0);
    int fileCount = 0;
    dialogProgress = new QProgressDialog(this);
    //**********
    threadCreatePro = std::make_shared<ProTreeThead>(std::ref(importPath),std::ref(path),
                                                     rightButtonItem,std::ref(fileCount),
                                                     this,rightButtonItem,nullptr);
    connect(threadCreatePro.get(),&ProTreeThead::SigUpdateProgress,
            this,&ProTreeWidget::SlotUpdateProgress);
    connect(threadCreatePro.get(),&ProTreeThead::SigFinishProgress,
            this,&ProTreeWidget::SlotFinishProgress);
    connect(dialogProgress,&QProgressDialog::canceled,
            this,&ProTreeWidget::SlotCancelProgress);
    connect(this,&ProTreeWidget::SigCancelProgress,
            threadCreatePro.get(),&ProTreeThead::SlotCancelProgress);
    threadCreatePro->start();
    dialogProgress->setWindowTitle("Please wait...");
    dialogProgress->setFixedWidth(PROGRESS_WIDTH);
    dialogProgress->setRange(0,PROGRESS_WIDTH);
    dialogProgress->exec();
}

void ProTreeWidget::SlotSetStart()
{
    if(!rightButtonItem)
        return;
    QFont nullFont;
    nullFont.setBold(false);
    if(activeItem){
        activeItem->setFont(0,nullFont);
    }
    activeItem = rightButtonItem;
    nullFont.setBold(true);
    activeItem->setFont(0,nullFont);

}

void ProTreeWidget::SlotClosePro()
{
    RemoveProDialog removeProDialog;
    int res = removeProDialog.exec();
    if(res != QDialog::Accepted){
        return;
    }
    bool bRemove = removeProDialog.isRomoved();
    int indexRightBtn = this->indexOfTopLevelItem(rightButtonItem);
    ProTreeItem *proTreeItem = dynamic_cast<ProTreeItem *>(rightButtonItem);
    QString deletePath = proTreeItem->GetPath();
    setPath.remove(deletePath);
    if(bRemove){
        QDir deleteDir(deletePath);
        deleteDir.removeRecursively();
    }
    if(proTreeItem == activeItem){
        activeItem = nullptr;
    }

    //关闭项目后 把照片区域也关闭
    ProTreeItem *selectedItem = dynamic_cast<ProTreeItem *>(selectItem);
    if(selectItem && proTreeItem == selectedItem->GetRoot()){
        selectedItem = nullptr;
        selectItem = nullptr;
        emit SigClearSelected();
    }
    delete this->takeTopLevelItem(indexRightBtn);
    rightButtonItem = nullptr;
}

void ProTreeWidget::SlotSlideShow()
{

    if(!rightButtonItem)
        return;
    ProTreeItem *rightProItem = dynamic_cast<ProTreeItem *>(rightButtonItem);
    ProTreeItem *lastChildItem = rightProItem->GetLastPicChild();
    if(!lastChildItem)
        return;
    qDebug()<< "last child item name is " << lastChildItem->GetPath()<< endl;
    ProTreeItem *firstChildItem = rightProItem->GetFirstPicChild();
    if(!firstChildItem)
        return;

    m_slideShowDlg = std::make_shared<SlideShowDlg>(this,firstChildItem,lastChildItem);
    m_slideShowDlg->setModal(true);
    m_slideShowDlg->showMaximized();

}

void ProTreeWidget::SlotUpdateProgress(int count)
{
    if(!dialogProgress){
        return;
    }
    if(count >= PROGRESS_MAX){
        dialogProgress->setValue(count%PROGRESS_MAX);
    }else{
         dialogProgress->setValue(count);
    }
}

void ProTreeWidget::SlotFinishProgress()
{
    dialogProgress->setValue(PROGRESS_MAX);
    dialogProgress->deleteLater();
}

void ProTreeWidget::SlotCancelProgress()
{
    emit SigCancelProgress();
    delete  dialogProgress;
    dialogProgress = nullptr;
}

void ProTreeWidget::SlotUpdateOpenProgress(int count)
{
    if(!dialogProgressOpen){
        return;
    }
    if(count >= PROGRESS_MAX){
        dialogProgressOpen->setValue(count%PROGRESS_MAX);
    }else{
         dialogProgressOpen->setValue(count);
    }
}

void ProTreeWidget::SlotFinishOpenProgress()
{
    if(!dialogProgressOpen){
        return;
    }
    dialogProgressOpen->setValue(PROGRESS_MAX);
    delete  dialogProgressOpen;
    dialogProgressOpen = nullptr;
}

void ProTreeWidget::SlotOpenPro(const QString &path)
{
    if(setPath.find(path) != setPath.end()){
        return;
    }
    setPath.insert(path);
    int fileCount = 0;
    QDir proDir(path);
    const QString &proName = proDir.dirName();
    m_openTreeThread = std::make_shared<OpenTreeThread>(path,fileCount,this,nullptr);
    m_openTreeThread->start();

    dialogProgressOpen = new QProgressDialog(this);

    //连接更新进度框操作
    connect(m_openTreeThread.get(), &OpenTreeThread::SigUpdateProgress,
            this, &ProTreeWidget::SlotUpdateOpenProgress);

    connect(m_openTreeThread.get(), &OpenTreeThread::SigFinishProgress, this,
            &ProTreeWidget::SlotFinishOpenProgress);

    dialogProgressOpen->setWindowTitle("Please wait...");
    dialogProgressOpen->setFixedWidth(PROGRESS_WIDTH);
    dialogProgressOpen->setRange(0, PROGRESS_MAX);
    dialogProgressOpen->exec();

}

void ProTreeWidget::SlotNextShow()
{
    if(!selectItem){
        return;
    }
    ProTreeItem *curItem = dynamic_cast<ProTreeItem *>(selectItem)->GetNextItem();
    if(!curItem){
        return;
    }
    emit SigUpdatePic(curItem->GetPath());
    selectItem = curItem;
    this->setCurrentItem(curItem);
}

void ProTreeWidget::SlotPreShow()
{
    if(!selectItem){
        return;
    }
    ProTreeItem *curItem = dynamic_cast<ProTreeItem *>(selectItem)->GetPreItem();
    if(!curItem){
        return;
    }
    emit SigUpdatePic(curItem->GetPath());
    selectItem = curItem;
    this->setCurrentItem(curItem);
}

void ProTreeWidget::SlotSetMusic()
{
    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::ExistingFiles);
    fileDialog.setWindowTitle(tr("选择音乐文件"));
    fileDialog.setDirectory(QDir::currentPath());
    fileDialog.setViewMode(QFileDialog::Detail);
    fileDialog.setNameFilter("*.mp3");
    QStringList fileNames;
    if(fileDialog.exec()){
        fileNames = fileDialog.selectedFiles();
    }else{
        return;
    }
    if(fileNames.length() <= 0)
        return;
    m_playList->clear();
    for(auto fileName: fileNames){
        m_playList->addMedia(QUrl::fromLocalFile(fileName));
    }
    if(m_player->state() != QMediaPlayer::PlayingState){
        m_playList->setCurrentIndex(0);
    }

}

void ProTreeWidget::SlotStartMusic()
{
    m_player->play();
}

void ProTreeWidget::SlotStopMusic()
{
    m_player->stop();
}


void SlotCancelOpenProgress(){

}
