#ifndef PICSHOW_H
#define PICSHOW_H

#include <QDialog>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include "picbutton.h"


namespace Ui {
class picShow;
}

class picShow : public QDialog
{
    Q_OBJECT

public:
    explicit picShow(QWidget *parent = nullptr);
    ~picShow();
    void ReloadPic();
protected:
    bool event(QEvent *e) override;

private:
    void showPreNextBtn(bool bShow);
    Ui::picShow *ui;
    QPropertyAnimation *m_animationShowPre;
    QPropertyAnimation *m_animationShownext;
    bool m_bBtnVisible;
    QString m_selectedPath;
    QPixmap m_pixmap;

public slots:
    void SlotSelectItem(const QString &path);
    void SlotUpdatePic(const QString &path);
    void SlotClearItem();
signals:
    void SigNextClicked();
    void SigPreClicked();
};

#endif // PICSHOW_H
