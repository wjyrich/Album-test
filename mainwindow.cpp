#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QFileDialog>
#include <QTreeWidget>

#include "picshow.h"
#include "wizard.h"
#include "protree.h"
#include "protreewidget.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //this->setMinimumSize()
    ui->setupUi(this);
    QMenu *menuFile = menuBar()->addMenu(tr("文件(&F)"));

    //菜单中创建项目动作
    QAction *actionCreatePro = new QAction(QIcon(":/icon/createpro.png"),tr("创建项目"),this);
    actionCreatePro->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    menuFile->addAction(actionCreatePro);

    //打开项目
    QAction *actionOpenPro = new QAction(QIcon(":/icon/openpro.png"),tr("打开项目"),this);
    actionOpenPro->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    menuFile->addAction(actionOpenPro);

    //设置背景音乐
    QMenu *menuSet = menuBar()->addMenu(tr("设置(&S)"));
    QAction *actionMusic = new QAction(QIcon(":/icon/music.png"),tr("背景音乐"),this);
    actionMusic->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_M));
    menuSet->addAction(actionMusic);

    connect(actionCreatePro,&QAction::triggered,this,&MainWindow::SlotCreatePro);
    connect(actionOpenPro,&QAction::triggered,this,&MainWindow::SlotOpenPro);
    widgetProTree = new ProTree();
    ui->treeLayout->addWidget(widgetProTree);

    //不能直接进行槽函数连接，需要进行转换
    QTreeWidget *treeWidget = dynamic_cast<ProTree *>(widgetProTree)->getTreeWidget();
    ProTreeWidget *proTreeWidget = dynamic_cast<ProTreeWidget *>(treeWidget);
    connect(this,&MainWindow::SigOpenPro,proTreeWidget,&ProTreeWidget::SlotOpenPro);

    m_picShow = new picShow();
    ui->photoLayout->addWidget(m_picShow);

    picShow *proPicShow = dynamic_cast<picShow *>(m_picShow);
    connect(proTreeWidget,&ProTreeWidget::SigUpdateSelected,proPicShow,&picShow::SlotSelectItem);
    connect(proPicShow,&picShow::SigNextClicked,proTreeWidget,&ProTreeWidget::SlotNextShow);
    connect(proPicShow,&picShow::SigPreClicked,proTreeWidget,&ProTreeWidget::SlotPreShow);
    connect(proTreeWidget,&ProTreeWidget::SigUpdatePic,proPicShow,&picShow::SlotUpdatePic);
    connect(proTreeWidget,&ProTreeWidget::SigClearSelected,proPicShow,&picShow::SlotClearItem);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    picShow *proPicShow = dynamic_cast<picShow *>(m_picShow);
    proPicShow->ReloadPic();
    QMainWindow::resizeEvent(event);
}

void MainWindow::SlotCreatePro()
{
    qDebug()<<"create succeed";
    Wizard wizard(this);
    wizard.setWindowTitle(tr("创建项目"));
    QWizardPage *page =  wizard.page(0);
    page->setTitle(tr("配置项目"));
    connect(&wizard,&Wizard::SigProSettings,dynamic_cast<ProTree *>(widgetProTree),&ProTree::addProToTree);

    wizard.show();
    wizard.exec();
}

void MainWindow::SlotOpenPro()
{
    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::Directory);
    fileDialog.setWindowTitle("选择导入的文件夹");
    fileDialog.setDirectory(QDir::currentPath());
    fileDialog.setViewMode(QFileDialog::Detail);
    QStringList fileNames;
    if(fileDialog.exec()){
        fileNames = fileDialog.selectedFiles();
    }
    if(fileNames.length() <= 0){
        return;
    }
    QString importPath = fileNames.at(0);
    emit SigOpenPro(importPath);
}

