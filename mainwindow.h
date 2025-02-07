#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    virtual void resizeEvent(QResizeEvent *event);

private:
    Ui::MainWindow *ui;
    QWidget *widgetProTree;
    QWidget *m_picShow;
private slots:
    void SlotCreatePro();
    void SlotOpenPro();
signals:
    void SigOpenPro(const QString &path);
};
#endif // MAINWINDOW_H
