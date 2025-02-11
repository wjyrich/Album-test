#include "prilistwid.h"

#include <QPainter>
#include <QGuiApplication>

#include "protreeitem.h"
#include "const.h"
#include "prelistitem.h"

PriListWid::PriListWid(QWidget *parent):QListWidget(parent),m_global(0),m_lastIndex(17)
{
    this->setViewMode(QListWidget::IconMode);
    this->setIconSize(QSize(PREICON_SIZE,PREICON_SIZE));
    this->setSpacing(5);
    connect(this,&PriListWid::itemPressed,this,&PriListWid::SlotItemPressed);
}

PriListWid::~PriListWid()
{

}

void PriListWid::AddListItem(const QString &path)
{
    QPixmap srcPixmap(path);
    srcPixmap = srcPixmap.scaled(PREICON_SIZE,PREICON_SIZE,Qt::KeepAspectRatioByExpanding);
    QPixmap distPixmap(QSize(PREICON_SIZE,PREICON_SIZE));
    distPixmap.fill(QColor(220,220,220,50));
    QPainter painter(&distPixmap);
    int distcWidth = distPixmap.width();
    int distHeight = distPixmap.height();

    int srcWidth = srcPixmap.width();
    int srcHeight = srcPixmap.height();
    int x = (distcWidth - srcWidth)/2;
    int y = (distHeight -srcHeight)/2;
    painter.drawPixmap(x,y,srcPixmap);
    m_global++;
    PreListItem *pItem = new PreListItem(QIcon(distPixmap),path,m_global,this);
    pItem->setSizeHint(QSize(PREICON_SIZE,PREICON_SIZE));
    this->addItem(pItem);
    m_setItems[path] = pItem;
    if(m_global == 1){
        m_posOrigin = this->pos();
    }
}

void PriListWid::SlotUpPreList(QTreeWidgetItem *treeItem)
{
    if(!treeItem){
        return;
    }
    ProTreeItem *proItem = dynamic_cast<ProTreeItem *>(treeItem);
    QString path = proItem->GetPath();
    auto iter = m_setItems.find(path);
    if(iter != m_setItems.end()){
        return;
    }

    AddListItem(path);
}

void PriListWid::SlotSelectItem(QTreeWidgetItem *treeItem)
{
    if(!treeItem)
        return;
    ProTreeItem *proItem = dynamic_cast<ProTreeItem *>(treeItem);
    QString path = proItem->GetPath();
    auto iter = m_setItems.find(path);
    if(iter == m_setItems.end()){
        return;
    }
    PreListItem *listItem = dynamic_cast<PreListItem *>(iter.value());
    int index = listItem->GetIndex();
    if(index > 17){
        QPoint posCur = this->pos();
        this->move(posCur.x() - (index -m_lastIndex)*100,posCur.y());
    }else{
        this->move(m_posOrigin);
        m_lastIndex = 17;
    }
    this->setCurrentItem(iter.value());
}

void PriListWid::SlotItemPressed(QListWidgetItem *item)
{
    if(QGuiApplication::mouseButtons() != Qt::LeftButton){
        return;
    }
    PreListItem *listItem = dynamic_cast<PreListItem *>(item);
    QString path = listItem->GetPath();
    this->setCurrentItem(item);
    emit SigupSelectShow(path);
}
