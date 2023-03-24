#ifndef DIRECTORYDIALOG_H
#define DIRECTORYDIALOG_H

#include <QDialog>
#include <QMap>
#include <QAbstractButton>
#include "filescanworker.h"


namespace Ui {
class DirectoryDialog;
}

class DirectoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DirectoryDialog(QWidget *parent = nullptr,  const QList<TElm> & dirList = {});
    ~DirectoryDialog();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

    void on_tableWidget_cellDoubleClicked(int row, int column);

private:
    Ui::DirectoryDialog *ui;
    void calcDirectories(const QList<TElm> & dirList);
    QMap<QString, TDirStat> m_dirStat;
};

#endif // DIRECTORYDIALOG_H
