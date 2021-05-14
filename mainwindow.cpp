#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>

#include <QProcess>
#include <QWindow>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_extensions = {"jpg", "jpeg", "png", "bmp"};

    ui->progressBar->setVisible(false);
}

MainWindow::~MainWindow()
{
    file_scan_thread.quit();
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


    int i = 0;

    // QList::
    foreach (const TElm &row, m_scanWorker->dirList()) {
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText(row.parent_dir);
        item->setData(Qt::UserRole, m_scanWorker->dirList().size() - 1);
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
        // item->setText(row.date.toString(Qt::ISODate));
        item->setData(Qt::DisplayRole, row.date);
        ui->tableWidget->setItem(i, 4, item);

        i++;
    }
    ui->tableWidget->setSortingEnabled(true);

    delete m_scanWorker;
    m_scanWorker = nullptr;
}


void MainWindow::on_pushButtonSelectDir_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    // QMessageBox::about(this, "Открыто", dir);
    this->ui->dirNameEdit->setText(dir);
    QDir directory(dir);

    QStringList images = directory.entryList(QStringList() << "*.jpg" << "*.JPG" << "*.PNG" << "*.png",QDir::Files);

}

void MainWindow::on_pushButtonStart_clicked()
{
    ui->progressBar->setVisible(true);
    ui->pushButtonStart->setEnabled(false);

    // get path name and scan
    m_scanWorker = new FileScanWorker(this->ui->dirNameEdit->text(), m_extensions);
    m_scanWorker->moveToThread(&file_scan_thread);

    connect(&file_scan_thread, SIGNAL(started()), m_scanWorker, SLOT(beginScan()));
    connect(m_scanWorker, SIGNAL(finish()), this, SLOT(updateScanResult()));
    file_scan_thread.start();
}
