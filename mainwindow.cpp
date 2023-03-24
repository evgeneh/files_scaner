#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>

#include <QProcess>
#include <QWindow>
#include <QDebug>

#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QIODevice>

#include "./utils/datedelegate.h"

#include "dialogs/assertiondialog.h"
#include "dialogs/directorydialog.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_dirsList()
    , m_recentDirs()
{
    ui->setupUi(this);

    m_extensions = {"jpg", "jpeg", "png", "bmp"};

    ui->progressBar->setVisible(false);

    loadDirHistory();
}

MainWindow::~MainWindow()
{
    delete ui;
}


//--------------------
void MainWindow::on_pushButton_3_clicked()
{
    QProcess *calcProc  = new QProcess;
    calcProc->start ("xcalc");
    if (calcProc->waitForStarted ()) {
      calcProc->waitForReadyRead (100); // just for timeout
      QProcess proc;
      proc.start ("xwininfo", QStringList () << "-int" << "-name" << "Calculator");
      proc.waitForReadyRead ();
      QByteArray in = proc.readAllStandardOutput ();
      qDebug() << in; //????????????
      proc.close ();

      int beg = in.indexOf ("id: ") + 4;
      int end = in.indexOf (" \"Calc");
      QByteArray wId = in.mid (beg, end-beg);

      QWindow *win = QWindow::fromWinId (WId(wId.toInt ()));
      win->setFlags(Qt::FramelessWindowHint);
      QWidget *cont = QWidget::createWindowContainer (win, NULL);
      ui->widget->layout()->addWidget (cont);
    }
}
//--------------------


void MainWindow::on_pushButton_2_clicked()
{
//
}


void MainWindow::on_dirNameEdit_textChanged(const QString &arg1)
{
    //
}


void MainWindow::on_pushButton_clicked()
{
    //
}

void MainWindow::updateScanResult()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(m_scanWorker->dirList().length());

    ui->progressBar->setVisible(false);
    ui->pushButtonStart->setEnabled(true);

    m_dirsList =  m_scanWorker->dirList();

    int i = 0;

    // QList::
    foreach (const TElm &row, m_scanWorker->dirList()) {
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText(row.parent_dir);
        item->setData(Qt::UserRole, (long long) row.id);
        ui->tableWidget->setItem(i, 0, item);

        item = new QTableWidgetItem();
        item->setText(row.dir);
        ui->tableWidget->setItem(i, 1, item);

        item = new QTableWidgetItem();
        item->setText(row.name);
        ui->tableWidget->setItem(i, 2, item);

        item = new QTableWidgetItem();
        item->setText(QString::number(row.size / 1024.0)); // size of file in Mb
        ui->tableWidget->setItem(i, 3, item);

        item = new QTableWidgetItem();
        item->setData(Qt::DisplayRole, row.date);
        ui->tableWidget->setItem(i, 4, item);

        i++;
    }
    ui->tableWidget->setSortingEnabled(true);
    DateDelegate *delegate = new DateDelegate(this);
    ui->tableWidget->setItemDelegateForColumn(4, delegate);

    m_scanWorker->deleteLater();
    m_scanWorker = nullptr;
}


void MainWindow::on_pushButtonSelectDir_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    this->ui->dirNameEdit->setText(dir);
    QDir directory(dir);

    QStringList images = directory.entryList(QStringList() << "*.jpg" << "*.JPG" << "*.PNG" << "*.png", QDir::Files);

}

void MainWindow::on_pushButtonStart_clicked()
{
    QString dirPath = this->ui->dirNameEdit->text();

    if (dirPath.length() < 2) {
        return;
    }

    QDir directory(dirPath);
    if (!directory.exists()) {
        QMessageBox::critical(this, "Error", "Directory \"" + dirPath + "\" is not exists");
        return;
    }

    ui->progressBar->setVisible(true);
    ui->pushButtonStart->setEnabled(false);

    QThread* file_scan_thread = new QThread;

    // get path name and scan
    m_scanWorker = nullptr;
    m_scanWorker = new FileScanWorker(this->ui->dirNameEdit->text(), m_extensions);

    m_scanWorker->moveToThread(file_scan_thread);

    connect(file_scan_thread, SIGNAL(started()), m_scanWorker, SLOT(beginScan()));
    connect(m_scanWorker, SIGNAL(finish()), this, SLOT(updateScanResult()));

    // connect(m_scanWorker, SIGNAL(finish()), m_scanWorker, SLOT(deleteLater()));
    connect(file_scan_thread, SIGNAL(finished()), file_scan_thread, SLOT(deleteLater()));
    file_scan_thread->start();

    // create history record
    if (m_recentDirs.contains(this->ui->dirNameEdit->text())) {
        m_recentDirs[this->ui->dirNameEdit->text()] = QDateTime::currentDateTime();
    } else {
        m_recentDirs.insert(this->ui->dirNameEdit->text(), QDateTime::currentDateTime());
    }
    updateHistroyDropList();
}

// Open directory or file by table cell click
void MainWindow::on_tableWidget_cellDoubleClicked(int row, int column)
{
   // QMessageBox::about(this, "Clicked", "ROW: " + QString::number(row) + "   COL: " + QString::number(column));
    bool ok = false;
    auto itemId = (long)ui->tableWidget->item(row, 0)->data(Qt::UserRole).toLongLong(&ok); // itemAt(row, 0)->data(Qt::UserRole).toLongLong();
    if (!ok) {
        return;
    }

    TElm elm = getElmById(itemId);
    if (elm.id < 0) {
        return;
    }
    if (column == 2)
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(elm.path + "/" + elm.name));
    }
    else if (column == 1)
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(elm.path));
    }
}

void MainWindow::on_actionContent_assertion_triggered()
{
    if (m_dirsList.isEmpty())
        return;
    AssertionDialog dialog(this, m_dirsList);
    dialog.exec();

}

void MainWindow::on_actionDirectories_changed()
{

}

void MainWindow::on_actionDirectories_triggered()
{
    // Show directories dialog
    DirectoryDialog dialog(this, m_dirsList);
    dialog.exec();
}

TElm MainWindow::getElmById(long id) const
{
    for (long i = 0; i < m_dirsList.length(); ++i) {
        if (m_dirsList.at(i).id == id) {
            return m_dirsList.at(i);
        }
    }
    return TElm();
}

// load recent directories
void MainWindow::loadDirHistory()
{
    QString filename(QDir::currentPath() + "/recent.log");
    if (!QFile::exists(filename)) {
        return;
    }
    QFile file(filename);

    if(file.open(QIODevice::ReadOnly)) {
        QDataStream out(&file);
        // out.setVersion(QDataStream::Qt_4_8);
        out >> m_recentDirs;
        file.close();
    }

    updateHistroyDropList();

    ui->dirNameEdit->setText(ui->comboBox_recentDirs->itemData(0, Qt::UserRole).toString());
}

void MainWindow::saveDirHistory()
{
    QString filename(QDir::currentPath() + "/recent.log");
    QFile file(filename);

    if(file.open(QIODevice::ReadWrite)) {
        QDataStream out(&file);
        // out.setVersion(QDataStream::Qt_4_8);
        out << m_recentDirs;
        file.close();
    }
}

void MainWindow::updateHistroyDropList()
{
    if (m_recentDirs.isEmpty()) {
        return;
    }
    ui->comboBox_recentDirs->clear();

    QList<QPair<QDateTime, QString>> sortedList;
    for (auto it = m_recentDirs.begin(); it != m_recentDirs.end(); ++it) {
        sortedList.append(QPair<QDateTime, QString>(it.value(), it.key()));
    }
    std::sort(sortedList.begin(), sortedList.end());

    for (int i = sortedList.length() - 1; i >= 0; --i) {
        ui->comboBox_recentDirs->addItem(sortedList.at(i).second + "   Date: <" + sortedList.at(i).first.toString("dd.MM.yyyy hh:mm")  + ">", sortedList.at(i).second);
    }
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    Q_UNUSED(event);
    saveDirHistory();
}

void MainWindow::on_MainWindow_destroyed()
{

}

void MainWindow::on_pushButton_addRecent_clicked()
{
    ui->dirNameEdit->setText(ui->comboBox_recentDirs->currentData().toString());
}

void MainWindow::on_comboBox_recentDirs_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    ui->dirNameEdit->setText(ui->comboBox_recentDirs->currentData().toString());
}
