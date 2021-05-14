#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QDateTime>

#include <QProcess>
#include <QWindow>
#include <QDebug>



struct TElm {
    TElm(long _id, QString _name, QDateTime _date, QDateTime _last_read, QString _path, QString _dir, QString _parent, qint64 _size):
        id(_id), name(_name), date(_date), last_read(_last_read), path(_path), dir(_dir), parent_dir(_parent), size(_size)
    { }
    long id;
    QString name;
    QDateTime date;
    QDateTime last_read;
    QString path;
    QString dir;
    QString parent_dir;
    QString checksum;
    qint64 size;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_extensions = {"jpg", "jpeg", "png", "bmp"};
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
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    // QMessageBox::about(this, "Открыто", dir);
    this->ui->dirNameEdit->setText(dir);
    QDir directory(dir);

    QStringList images = directory.entryList(QStringList() << "*.jpg" << "*.JPG" << "*.PNG" << "*.png",QDir::Files);
    foreach(QString filename, images) {
    //do whatever you need to do
    }
}


void MainWindow::on_dirNameEdit_textChanged(const QString &arg1)
{
    //
}


// рекурсивный поиск в файловой системе
bool FindFileAndDir( QString szDir)
{
     qint16 qnFileCount = 0;
     QDir dir( szDir);
     foreach( QFileInfo fi, dir.entryInfoList())
     {
         QString szFileName = fi.absoluteFilePath();
         if( fi.isDir())
         {
             if( fi.fileName()=="." || fi.fileName()=="..")
             continue;

            FindFileAndDir( szFileName);
         }
         if( fi.isFile())
         {
            qnFileCount++;
         }
     }
     qDebug( "Dir: %s, File count: %d", qPrintable( szDir), qnFileCount);

     return true;
}


QList<TElm> dirsList;
long dirCount;
long fileCount;


void scanDir(QString dir_path, QString parent) {
    QDir directory(dir_path);
    QFileInfoList dirItemsVector;
    if (!directory.exists()){
        return;
    }
    dirItemsVector = directory.entryInfoList();

    if (dirItemsVector.size() > 2)
    {
        foreach (const QFileInfo dir_item, dirItemsVector) {
            if (dir_item.fileName() != "." && dir_item.fileName() != ".." ) {
                if (dir_item.isDir()) {
                    // parse as dir
                    if (! dir_item.fileName().contains("_files"))
                    {
                        dirCount++;
                        scanDir(dir_item.filePath(), directory.dirName());
                    }
                } else if (dir_item.isFile()){
                    //parse as file
                    // if (m_extensions dir_item.completeSuffix();

                    QDateTime d_created = dir_item.created();
                    QDateTime d_last_read = dir_item.lastRead();
                    QString dir =  directory.dirName().section('\\', -1);
                    fileCount++;
                    TElm elm{fileCount, dir_item.fileName(), d_created, d_last_read, dir_item.path(), dir, parent, dir_item.size()};
                    dirsList << elm;
                }

            }
        }
    }
}


void MainWindow::on_pushButton_clicked()
{
    dirCount = 0;
    fileCount = 0;
    // get path name and scan
    scanDir(this->ui->dirNameEdit->text(), "__ROOT__");

    // dirsList
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(dirsList.length());
    int i = 0;
    foreach (const TElm &row, dirsList) {
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText(row.parent_dir);
        item->setData(Qt::UserRole, dirsList.size() - 1);
        ui->tableWidget->setItem(i, 0, item);

        item = new QTableWidgetItem();
        item->setText(row.dir);
        ui->tableWidget->setItem(i, 1, item);

        item = new QTableWidgetItem();
        item->setText(row.name);
        ui->tableWidget->setItem(i, 2, item);

        item = new QTableWidgetItem();
        item->setText(QString::number(row.size / 1000.0)); // size of file in Mb
        ui->tableWidget->setItem(i, 3, item);

        item = new QTableWidgetItem();
        item->setText(row.date.toString(Qt::ISODate));
        ui->tableWidget->setItem(i, 4, item);

        i++;
    }
}

