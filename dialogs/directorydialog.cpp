#include "directorydialog.h"
#include "ui_directorydialog.h"
#include "../utils/datedelegate.h"


DirectoryDialog::DirectoryDialog(QWidget *parent, const QList<TElm> & dirList) :
    QDialog(parent),
    ui(new Ui::DirectoryDialog)
{
    ui->setupUi(this);
    calcDirectories(dirList);
}

DirectoryDialog::~DirectoryDialog()
{
    delete ui;
}

void DirectoryDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    accept();
}

void DirectoryDialog::on_buttonBox_rejected()
{
    reject();
}

void DirectoryDialog::on_buttonBox_accepted()
{
    accept();
}

void DirectoryDialog::calcDirectories(const QList<TElm> &dirList)
{
    // ui->tableWidget->setRowCount(dirList.length());

    QMap<QString, TDirStat> dirStat;
    int k = 0;
    foreach (TElm row, dirList) {
        if (dirStat.contains(row.path)) {
            dirStat[row.path].updateDir(row);
        } else {            
            dirStat.insert(row.path, TDirStat(row));
            ++k;
        }
    }

    ui->tableWidget->setRowCount(k);

    int i = 0;
    auto it = dirStat.constBegin();
    for (it = dirStat.constBegin(); it != dirStat.constEnd(); ++it)
    {
        auto row = it.value();
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText(row.parent);
        item->setData(Qt::UserRole, (it.key()));
        ui->tableWidget->setItem(i, 0, item);

        item = new QTableWidgetItem();
        item->setText(row.name);
        ui->tableWidget->setItem(i, 1, item);

        item = new QTableWidgetItem();
        item->setData(Qt::DisplayRole, row.elmCount);
        ui->tableWidget->setItem(i, 2, item);

        item = new QTableWidgetItem();
        item->setData(Qt::DisplayRole, row.totalSize / 1024.0); // size of file in Mb
        ui->tableWidget->setItem(i, 3, item);

        item = new QTableWidgetItem();
        item->setData(Qt::DisplayRole, row.minDate);
        ui->tableWidget->setItem(i, 4, item);

        item = new QTableWidgetItem();
        // item->setText(row.maxDate.toString("dd.MM.yyyy hh:mm"));
        item->setData(Qt::DisplayRole, row.maxDate);
        ui->tableWidget->setItem(i, 5, item);

        i++;
    }
    DateDelegate *delegate = new DateDelegate(this);
    ui->tableWidget->setItemDelegateForColumn(4, delegate);
    ui->tableWidget->setItemDelegateForColumn(5, delegate);

    ui->tableWidget->setSortingEnabled(true);
}
