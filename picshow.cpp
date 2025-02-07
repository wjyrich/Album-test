#include "picshow.h"
#include "ui_picshow.h"

#include <QEvent>
picShow::picShow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::picShow)
{
    ui->setupUi(this);
    ui->previous->SetIcons(":/icon/previous.png"
                           ,":/icon/previous_hover.png"
                           ,":/icon/previous_press.png");
    ui->next->SetIcons(":/icon/next.png"
                           ,":/icon/next_hover.png"
                           ,":/icon/next_press.png");
    QGraphicsOpacityEffect *opacityPre = new QGraphicsOpacityEffect(this);
    opacityPre->setOpacity(0);
    ui->previous->setGraphicsEffect(opacityPre);
    QGraphicsOpacityEffect *opacitynext = new QGraphicsOpacityEffect(this);
    opacitynext->setOpacity(0);
    ui->next->setGraphicsEffect(opacitynext);

    m_animationShowPre = new QPropertyAnimation(opacityPre,"opacity",this);
    m_animationShowPre->setEasingCurve(QEasingCurve::Linear);
    m_animationShowPre->setDuration(500);

    m_animationShownext = new QPropertyAnimation(opacitynext,"opacity",this);
    m_animationShownext->setEasingCurve(QEasingCurve::Linear);
    m_animationShownext->setDuration(500);
    connect(ui->next,&QPushButton::clicked,this,&picShow::SigNextClicked);
    connect(ui->previous,&QPushButton::clicked,this,&picShow::SigPreClicked);

}

picShow::~picShow()
{
    delete ui;
}

void picShow::ReloadPic()
{
    if(m_selectedPath != ""){
        const int &width = ui->gridLayout->geometry().width();
        const int &height = ui->gridLayout->geometry().height();
        m_pixmap.load(m_selectedPath);
        m_pixmap = m_pixmap.scaled(width,height,Qt::KeepAspectRatio);
        ui->label->setPixmap(m_pixmap);
    }
}

bool picShow::event(QEvent *e)
{
    switch (e->type()) {
    case QEvent::Enter:
            showPreNextBtn(true);
            break;
    case QEvent::Leave:
        showPreNextBtn(false);
        break;
    default:
        break;
    }
    return QDialog::event(e);
}

void picShow::showPreNextBtn(bool bShow)
{
    if(!bShow && m_bBtnVisible){
        m_animationShowPre->stop();
        m_animationShowPre->setStartValue(1);
        m_animationShowPre->setEndValue(0);
        m_animationShowPre->start();

        m_animationShownext->stop();
        m_animationShownext->setStartValue(1);
        m_animationShownext->setEndValue(0);
        m_animationShownext->start();
        m_bBtnVisible = false;

        return;
    }
    if(bShow && !m_bBtnVisible){
        m_animationShowPre->stop();
        m_animationShowPre->setStartValue(0);
        m_animationShowPre->setEndValue(1);
        m_animationShowPre->start();

        m_animationShownext->stop();
        m_animationShownext->setStartValue(0);
        m_animationShownext->setEndValue(1);
        m_animationShownext->start();
        m_bBtnVisible = true;

        return;
    }
}

void picShow::SlotSelectItem(const QString &path)
{
    m_selectedPath = path;
    m_pixmap.load(path);
    int width = this->width()-20;
    int height = this->height()-20;
    m_pixmap = m_pixmap.scaled(width,height,Qt::KeepAspectRatio);
    ui->label->setPixmap(m_pixmap);

}

void picShow::SlotUpdatePic(const QString &path)
{
    m_selectedPath = path;
    if(m_selectedPath != ""){
        const int &width = ui->gridLayout->geometry().width();
        const int &height = ui->gridLayout->geometry().height();
        m_pixmap.load(m_selectedPath);
        m_pixmap = m_pixmap.scaled(width,height,Qt::KeepAspectRatio);
        ui->label->setPixmap(m_pixmap);
    }
}

void picShow::SlotClearItem()
{
    m_selectedPath = "";
}
