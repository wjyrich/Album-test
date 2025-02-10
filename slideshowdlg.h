#ifndef SLIDESHOWDLG_H
#define SLIDESHOWDLG_H

#include <QDialog>
#include <QTreeWidgetItem>

namespace Ui {
class SlideShowDlg;
}

class SlideShowDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SlideShowDlg(QWidget *parent = nullptr, QTreeWidgetItem *firstItem = nullptr, QTreeWidgetItem *lastItem = nullptr);
    ~SlideShowDlg();

private:
    Ui::SlideShowDlg *ui;
    QTreeWidgetItem *m_firstItem;
    QTreeWidgetItem *m_lastItem;
};

#endif // SLIDESHOWDLG_H
