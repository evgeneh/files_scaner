#ifndef ASSERTIONDIALOG_H
#define ASSERTIONDIALOG_H

#include <QDialog>
#include <QList>
#include "filescanworker.h"

namespace Ui {
class AssertionDialog;
}

class AssertionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AssertionDialog(QWidget *parent = nullptr, const QList<TElm> & dirList = {});
    ~AssertionDialog();

private slots:
    void on_buttonBox_accepted();

    void on_horizontalSlider_valueChanged(int value);

    void on_toolButtonReverse_triggered(QAction *arg1);

    void on_pushButtonReverse_clicked();

private:
    Ui::AssertionDialog *ui;
    QList<TElm> m_dirsList;
    QDateTime m_currentDate;

    void calcDirCounts();
    bool filterTime(const QDateTime &value);
};

#endif // ASSERTIONDIALOG_H
