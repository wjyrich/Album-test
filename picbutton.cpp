#include "picbutton.h"


picButton::picButton(QWidget *parent)
{

}

void picButton::SetIcons(const QString &normal, const QString &hover, const QString &pressed)
{
    m_normal = normal;
    m_hover = hover;
    m_pressed = pressed;
    QPixmap  tmpPixmap;
    tmpPixmap.load(normal);
    this->resize(tmpPixmap.size());
    this->setIcon(tmpPixmap);
    this->setIconSize(tmpPixmap.size());

}

bool picButton::event(QEvent *e)
{
    switch (e->type()) {
        case QEvent::Enter:
            setHover();
            break;
        case QEvent::Leave:
            setNormalIcon();
            break;
        case QEvent::MouseButtonPress:
            setPressed();
            break;
        case QEvent::MouseButtonRelease:
            setNormalIcon();
            break;
        default:
            break;
    }
    return QPushButton::event(e);
}

void picButton::setNormalIcon()
{
    QPixmap  tmpPixmap;
    tmpPixmap.load(m_normal);
    this->setIcon(tmpPixmap);
}

void picButton::setHover()
{
    QPixmap  tmpPixmap;
    tmpPixmap.load(m_hover);
    this->setIcon(tmpPixmap);
}

void picButton::setPressed()
{
    QPixmap  tmpPixmap;
    tmpPixmap.load(m_pressed);
    this->setIcon(tmpPixmap);
}
