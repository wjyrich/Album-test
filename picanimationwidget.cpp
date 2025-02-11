#include "picanimationwidget.h"
#include <QTimer>
#include "protreeitem.h"
#include <QPainter>
PicAnimationWidget::PicAnimationWidget(QWidget *parent) : QWidget(parent)
  , m_factor(0.0),m_curItem(nullptr),m_bStart(false)

{
    m_timer = new QTimer(this);
    connect(m_timer,&QTimer::timeout,this,&PicAnimationWidget::TimeOut);

}

PicAnimationWidget::~PicAnimationWidget()
{

}

void PicAnimationWidget::SetPixmap(QTreeWidgetItem *item)
{
    if(!item)
        return;
    ProTreeItem *treeItem = dynamic_cast<ProTreeItem *>(item);
    QString path = treeItem->GetPath();
    m_pixmap1.load(path);
    m_curItem = treeItem;

    if(m_mapItem.find(path) == m_mapItem.end()){
        m_mapItem[path] = treeItem;
        //发送更新列表逻辑
        emit SigUpPreList(item);
    }
    emit SigSlectItem(item);
    ProTreeItem *nextItem = treeItem->GetNextItem();
    if(!nextItem)
        return;
    QString nextPath = nextItem->GetPath();
    m_pixmap2.load(nextPath);
    if(m_mapItem.find(nextPath) == m_mapItem.end()){
        m_mapItem[nextPath] = nextItem;
        //发送更新列表逻辑
        emit SigUpPreList(nextItem);
    }
}

void PicAnimationWidget::Start()
{
    emit SigStart();
    emit SigStartMusic();
    m_factor = 0;
    m_timer->start(25);
    m_bStart = true;
}

void PicAnimationWidget::Stop()
{
   emit SigStop();
   emit SigStopMusic();
   m_timer->stop();
   m_factor = 0;
   m_bStart = false;

}

void PicAnimationWidget::paintEvent(QPaintEvent *event)
{
    if(m_pixmap1.isNull())
        return;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    QRect rect = geometry();
    int w = rect.width();
    int h = rect.height();
    m_pixmap1 = m_pixmap1.scaled(w,h,Qt::KeepAspectRatio);
    int alpha = 255*(1.0f-m_factor);
    QPixmap alphaPixmap(m_pixmap1.size());
    alphaPixmap.fill(Qt::transparent);
    QPainter p1(&alphaPixmap);
    p1.setCompositionMode(QPainter::CompositionMode_Source);
    p1.drawPixmap(0,0,m_pixmap1);
    p1.setCompositionMode(QPainter::CompositionMode_Destination);
    p1.fillRect(alphaPixmap.rect(),QColor(0,0,0,alpha));
    p1.end();

    int x = (w - m_pixmap1.width())/2;
    int y = (h - m_pixmap1.height())/2;
    painter.drawPixmap(x,y,alphaPixmap);

    if(m_pixmap2.isNull())
        return;
    m_pixmap2 = m_pixmap2.scaled(w,h,Qt::KeepAspectRatio);
    alpha = 255*(m_factor);
    QPixmap alphaPixmap2(m_pixmap2.size());
    alphaPixmap2.fill(Qt::transparent);
    QPainter p2(&alphaPixmap2);
    p2.setCompositionMode(QPainter::CompositionMode_Source);
    p2.drawPixmap(0, 0, m_pixmap2);
    p2.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    p2.fillRect(alphaPixmap2.rect(), QColor(0, 0, 0, alpha));
    p2.end();
     x = (w - m_pixmap2.width()) / 2;
     y = (h - m_pixmap2.height()) / 2;
     painter.drawPixmap(x, y, alphaPixmap2);
}

void PicAnimationWidget::updateSelectPixmap(QTreeWidgetItem *item)
{
    if(!item){
        return;
    }
    ProTreeItem *treeItem = dynamic_cast<ProTreeItem *>(item);
    QString path = treeItem->GetPath();
    m_pixmap1.load(path);
    m_curItem = treeItem;

    if(m_mapItem.find(path) == m_mapItem.end()){
        m_mapItem[path] = treeItem;
    }
    ProTreeItem *nextItem = treeItem->GetNextItem();
    if(!nextItem)
        return;
    QString nextPath = treeItem->GetPath();
    m_pixmap2.load(nextPath);
    if(m_mapItem.find(nextPath) == m_mapItem.end()){
        m_mapItem[path] = nextItem;
    }

}

void PicAnimationWidget::SlotUpSelectShow(QString path)
{
    auto iter = m_mapItem.find(path);
    if(iter == m_mapItem.end())
        return;
    updateSelectPixmap(iter.value());
    update();
}

void PicAnimationWidget::SlotPlayAndStop()
{
    if(!m_bStart){
        m_factor = 0;
        m_timer->start(25);
        m_bStart = true;
        emit SigStartMusic();
    }else{
        m_timer->stop();
        m_factor = 0;
        update();
        m_bStart = false;
        emit SigStopMusic();

    }
}

void PicAnimationWidget::SlideNext()
{
    Stop();
    if(!m_curItem){
        return;
    }
    ProTreeItem *curProItem = dynamic_cast<ProTreeItem *>(m_curItem);
    ProTreeItem *nextItem = curProItem->GetNextItem();
    if(!nextItem)
        return;
    SetPixmap(nextItem);
    update();
}

void PicAnimationWidget::SlidePre()
{
    Stop();
    if(!m_curItem)
        return;
    ProTreeItem *curProItem = dynamic_cast<ProTreeItem *>(m_curItem);
    ProTreeItem *PreItem = curProItem->GetPreItem();
    if(!PreItem)
        return;
    SetPixmap(PreItem);
    update();
}

void PicAnimationWidget::TimeOut()
{
    if(!m_curItem){
        Stop();
        update();
        return;
    }
    m_factor = m_factor + 0.01;
    if(m_factor >= 1){
        m_factor = 0;
        ProTreeItem *curProItem = dynamic_cast<ProTreeItem *>(m_curItem);
        ProTreeItem *nextProItem = curProItem->GetNextItem();
        if(!nextProItem){
            Stop();
            update();
            return;
        }
        SetPixmap(nextProItem);
        update();
        return;
    }
    update();
}
