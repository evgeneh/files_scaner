#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QList>
#include "filescanworker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_2_clicked();

    void on_dirNameEdit_textChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void updateScanResult();

    void on_pushButtonSelectDir_clicked();

    void on_pushButtonStart_clicked();

private:
    Ui::MainWindow *ui;

    QList<QString> m_extensions;

    FileScanWorker *m_scanWorker;

    QThread file_scan_thread;
};
#endif // MAINWINDOW_H
