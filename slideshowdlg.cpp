#include "slideshowdlg.h"
#include "ui_slideshowdlg.h"
#include "prilistwid.h"
#include "protreewidget.h"
SlideShowDlg::SlideShowDlg(QWidget *parent,QTreeWidgetItem *firstItem, QTreeWidgetItem *lastItem) :
    QDialog(parent),m_firstItem(firstItem),m_lastItem(lastItem),
    ui(new Ui::SlideShowDlg)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    ui->slidePreBtn->SetIcons(":/icon/previous.png",
                              ":/icon/previous_hover.png",
                              ":/icon/previous_press.png");

    ui->slideNextBtn->SetIcons(":/icon/next.png",
                              ":/icon/next_hover.png",
                              ":/icon/next_press.png");
    ui->pauseBtn->SetIcons(":/icon/closeshow.png",":/icon/closeshow_hover.png",
                           ":/icon/closeshow_press.png");

    ui->playBtn->SetIcons(":/icon/play.png",":/icon/play_hover.png",":/icon/play_press.png",
                          ":/icon/pause.png",":/icon/pause_hover.png",":/icon/pause_press.png");

    connect(ui->pauseBtn,&QPushButton::clicked,this,&SlideShowDlg::close);
    connect(ui->slideNextBtn,&QPushButton::clicked,this,&SlideShowDlg::SlotSlideNext);
    connect(ui->slidePreBtn,&QPushButton::clicked,this,&SlideShowDlg::SlotSlidePre);
    PriListWid *preListWid = dynamic_cast<PriListWid *>(ui->preListWidget);
    connect(ui->picAnimation,&PicAnimationWidget::SigUpPreList,preListWid,&PriListWid::SlotUpPreList);
    connect(ui->picAnimation,&PicAnimationWidget::SigSlectItem,preListWid,&PriListWid::SlotSelectItem);

    connect(preListWid,&PriListWid::SigupSelectShow,ui->picAnimation,&PicAnimationWidget::SlotUpSelectShow);
    connect(ui->playBtn,&QPushButton::clicked,ui->picAnimation,&PicAnimationWidget::SlotPlayAndStop);
    connect(ui->picAnimation,&PicAnimationWidget::SigStart,ui->playBtn,&PicStateButton::SlotStart);
    connect(ui->picAnimation,&PicAnimationWidget::SigStop,ui->playBtn,&PicStateButton::SlotStop);

    ProTreeWidget *treeWidget = dynamic_cast<ProTreeWidget *>(parent);
    connect(ui->picAnimation,&PicAnimationWidget::SigStartMusic,treeWidget,&ProTreeWidget::SlotStartMusic);
    connect(ui->picAnimation,&PicAnimationWidget::SigStopMusic,treeWidget,&ProTreeWidget::SlotStopMusic);

    ui->picAnimation->SetPixmap(firstItem);

    ui->picAnimation->Start();
}

SlideShowDlg::~SlideShowDlg()
{
    delete ui;
}

void SlideShowDlg::SlotSlideNext()
{
    ui->picAnimation->SlideNext();
}

void SlideShowDlg::SlotSlidePre()
{
    ui->picAnimation->SlidePre();
}
