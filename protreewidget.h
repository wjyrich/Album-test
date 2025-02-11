#ifndef PROTREEWIDGET_H
#define PROTREEWIDGET_H

#include <QTreeWidget>
#include <QAction>
#include <QProgressDialog>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaPlaylist>

class SlideShowDlg;

#include "protreethead.h"
#include "opentreethread.h"
class ProTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    ProTreeWidget(QWidget *parent = nullptr);
    void addProToTree(const QString &name, const QString &path);
private:
    QSet<QString> setPath;
    //右键item后弹出菜单
    QTreeWidgetItem *rightButtonItem;
    //当前的项目
    QTreeWidgetItem *activeItem;
    //记录当前的目录，就是进入下一个目录的时候
    QTreeWidgetItem *selectItem;
    QAction *actionImport;
    QAction *actionSetstart;
    QAction *actionClosePro;
    QAction *actionSlideShow;
    QProgressDialog *dialogProgress;
    QProgressDialog *dialogProgressOpen;
    std::shared_ptr<ProTreeThead> threadCreatePro;//智能指针
    std::shared_ptr<OpenTreeThread> m_openTreeThread;
    std::shared_ptr<SlideShowDlg> m_slideShowDlg;
    QMediaPlayer *m_player;
    QMediaPlaylist *m_playList;

private slots:
    void SlotItemPressed(QTreeWidgetItem *pressItem, int column);
    void SlotDoubleClickedItem(QTreeWidgetItem *doubleClickedItem,int col);
    void SlotImport();
    void SlotSetStart();
    void SlotClosePro();
    void SlotSlideShow();
    void SlotUpdateProgress(int count);
    void SlotFinishProgress();
    void SlotCancelProgress();

    void SlotUpdateOpenProgress(int count);
    void SlotFinishOpenProgress();

public slots:
    void SlotOpenPro(const QString &path);
    void SlotNextShow();
    void SlotPreShow();
    void SlotSetMusic();
    void SlotStartMusic();
    void SlotStopMusic();

signals:
    void SigCancelProgress();
    void SigUpdateSelected(const QString& );
    void SigUpdatePic(const QString& );
    void SigClearSelected();
};

#endif // PROTREEWIDGET_H
