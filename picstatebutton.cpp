
#include <QEvent>
#include "picstatebutton.h"
#include "const.h"

PicStateButton::PicStateButton(QWidget *parent):QPushButton(parent)
{

}

void PicStateButton::SetIcons(const QString &normal, const QString &hover, const QString &pressed, const QString &normal_2, const QString &hover_2, const QString &pressed_2)
{
    m_normal = normal;
    m_hover = hover;
    m_pressed = pressed;

    m_normal_2 = normal_2;
    m_hover_2 = hover_2;
    m_pressed_2 = pressed_2;

    QPixmap tmpPixmap;
    tmpPixmap.load(normal);
    this->resize(tmpPixmap.size());
    this->setIcon(tmpPixmap);
    this->setIconSize(tmpPixmap.size());
    m_curState = PicBtnStateNormal;
}

bool PicStateButton::event(QEvent *e)
{
    switch (e->type()) {
        case QEvent::Enter:
            if(m_curState < PicBtnState2Normal){
                setHoverIcon();
            }else{
                setHover2Icon();
            }

            break;
        case QEvent::Leave:
            if(m_curState < PicBtnState2Normal){
                setNormalIcon();
            }else{
                setHover2Icon();
            }

            break;
        case QEvent::MouseButtonPress:
            if(m_curState < PicBtnState2Normal){
                setPressIcon();
            }else{
                setPress2Icon();
            }

            break;
        case QEvent::MouseButtonRelease:
            if(m_curState < PicBtnState2Normal){
                setHover2Icon();
            }else{
                setHoverIcon();
            }
            break;
        default:
            break;
    }
    return QPushButton::event(e);
}

void PicStateButton::setNormalIcon()
{
    QPixmap tmpPixmap;
    tmpPixmap.load(m_normal);
    this->setIcon(tmpPixmap);
    m_curState = PicBtnStateNormal;
}

void PicStateButton::setHoverIcon()
{
    QPixmap tmpPixmap;
    tmpPixmap.load(m_hover);
    this->setIcon(tmpPixmap);
    m_curState = PicBtnStateHover;
}

void PicStateButton::setPressIcon()
{
    QPixmap tmpPixmap;
    tmpPixmap.load(m_pressed);
    this->setIcon(tmpPixmap);
    m_curState = PicBtnStatePress;
}

void PicStateButton::setNormal2Icon()
{
    QPixmap tmpPixmap;
    tmpPixmap.load(m_normal_2);
    this->setIcon(tmpPixmap);
    m_curState = PicBtnState2Normal;
}

void PicStateButton::setHover2Icon()
{
    QPixmap tmpPixmap;
    tmpPixmap.load(m_hover_2);
    this->setIcon(tmpPixmap);
    m_curState = PicBtnState2Hover;
}

void PicStateButton::setPress2Icon()
{
    QPixmap tmpPixmap;
    tmpPixmap.load(m_pressed_2);
    this->setIcon(tmpPixmap);
    m_curState = PicBtnState2Press;
}

void PicStateButton::SlotStart()
{
    setNormal2Icon();
}

void PicStateButton::SlotStop()
{
    setNormalIcon();
}
