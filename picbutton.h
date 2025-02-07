#ifndef PICBUTTON_H
#define PICBUTTON_H

#include <QPushButton>
#include <QPixmap>
#include <QEvent>
class picButton : public QPushButton
{
public:
    picButton(QWidget *parent = nullptr);
    void SetIcons(const QString& normal, const QString& hover, const QString& pressed);

protected:
    bool event(QEvent *e) override;

private:
    void setNormalIcon();
    void setHover();
    void setPressed();
    QString m_normal;
    QString m_hover;
    QString m_pressed;

};

#endif // PICBUTTON_H
