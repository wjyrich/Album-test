#ifndef PICANIMATIONWIDGET_H
#define PICANIMATIONWIDGET_H

#include <QObject>
#include <QWidget>
#include <QTreeWidgetItem>
class PicAnimationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PicAnimationWidget(QWidget *parent = nullptr);
    ~PicAnimationWidget();
    void SetPixmap(QTreeWidgetItem *item);
    void Start();
    void Stop();
    void SlideNext();
    void SlidePre();
protected:
    virtual void paintEvent(QPaintEvent *event);

signals:
    void SigUpPreList(QTreeWidgetItem *);
    void SigSlectItem(QTreeWidgetItem *);
    void SigStart();
    void SigStop();
    void SigStartMusic();
    void SigStopMusic();
private:
    void updateSelectPixmap(QTreeWidgetItem *item);
    float m_factor;
    QTimer *m_timer;
    QPixmap m_pixmap1;
    QPixmap m_pixmap2;
    QTreeWidgetItem * m_curItem;
    QMap<QString,QTreeWidgetItem*> m_mapItem;
    bool m_bStart;
public slots:
    void SlotUpSelectShow(QString path);
    void SlotPlayAndStop();
private slots:
    void TimeOut();
};

#endif // PICANIMATIONWIDGET_H
