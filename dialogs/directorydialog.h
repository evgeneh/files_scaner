#ifndef DIRECTORYDIALOG_H
#define DIRECTORYDIALOG_H

#include <QDialog>
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

private:
    Ui::DirectoryDialog *ui;
    void calcDirectories(const QList<TElm> & dirList);

};

#endif // DIRECTORYDIALOG_H
