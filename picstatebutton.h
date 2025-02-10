#ifndef PICSTATEBUTTON_H
#define PICSTATEBUTTON_H

#include <QPushButton>

class PicStateButton : public QPushButton
{
    Q_OBJECT
public:
    PicStateButton(QWidget *parent = nullptr);
    void SetIcons(const QString& normal, const QString& hover, const QString& pressed,
                  const QString& normal_2, const QString& hover_2, const QString& pressed_2);
protected:
    bool event(QEvent *e) override;

private:
    void setNormalIcon();
    void setHoverIcon();
    void setPressIcon();

    void setNormal2Icon();
    void setHover2Icon();
    void setPress2Icon();

    QString m_normal;
    QString m_hover;
    QString m_pressed;
    QString m_normal_2;
    QString m_hover_2;
    QString m_pressed_2;
    int m_curState;

};

#endif // PICSTATEBUTTON_H
