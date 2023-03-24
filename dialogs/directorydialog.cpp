#include "directorydialog.h"
#include "ui_directorydialog.h"
#include "../utils/datedelegate.h"

#include <QDesktopServices>
#include <QUrl>


DirectoryDialog::DirectoryDialog(QWidget *parent, const QList<TElm> & dirList):
    QDialog(parent),
    ui(new Ui::DirectoryDialog),
    m_dirStat()
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
    m_dirStat.clear();
    int k = 0;
    foreach (TElm row, dirList) {
        if (m_dirStat.contains(row.path)) {
            m_dirStat[row.path].updateDir(row);
        } else {            
            m_dirStat.insert(row.path, TDirStat(row));
            ++k;
        }
    }

    ui->tableWidget->setRowCount(k);

    int i = 0;
    auto it = m_dirStat.constBegin();
    for (it = m_dirStat.constBegin(); it != m_dirStat.constEnd(); ++it)
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

void DirectoryDialog::on_tableWidget_cellDoubleClicked(int row, int column)
{

    auto itemPath = ui->tableWidget->item(row, 0)->data(Qt::UserRole).toString(); // itemAt(row, 0)->data(Qt::UserRole).toLongLong();

    if (!m_dirStat.contains(itemPath)) {
        return;
    }

    TDirStat dirStatItem = m_dirStat[itemPath];

    QString pathToOpen = "";
    if (column == 1)
    {
        pathToOpen = itemPath;

    }
    else if (column == 4)
    {
        pathToOpen = itemPath + "/" + dirStatItem.oldestFileName;
    }
    else if (column == 5)
    {
        pathToOpen = itemPath + "/" + dirStatItem.latestFileName;
    }

    if (pathToOpen.length() > 1){
        QDesktopServices::openUrl(QUrl::fromLocalFile(pathToOpen));
    }
}
