#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QList>
#include <QMap>
#include <QDateTime>
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

    void on_tableWidget_cellDoubleClicked(int row, int column);

    void on_actionContent_assertion_triggered();

    void on_actionDirectories_changed();

    void on_actionDirectories_triggered();

    void on_MainWindow_destroyed();

    void closeEvent (QCloseEvent *event);

    void on_pushButton_addRecent_clicked();

    void on_comboBox_recentDirs_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;

    QList<QString> m_extensions;

    FileScanWorker *m_scanWorker;

    QList<TElm> m_dirsList;

    TElm getElmById(long id) const;

    QMap<QString, QDateTime> m_recentDirs;

    void loadDirHistory();
    void saveDirHistory();
    void updateHistroyDropList();
};
#endif // MAINWINDOW_H
