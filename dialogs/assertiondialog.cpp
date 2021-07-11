#include "assertiondialog.h"
#include "ui_assertiondialog.h"

#include <QCommonStyle>

struct TDirStatItem {
    TDirStatItem(QString _path, QString _dir, QString _parent):
        count(1), path(_path), dir(_dir), parent_dir(_parent)
    { }

    void inc() {
        this->count += 1;
    }

    long count;
    QString path;
    QString dir;
    QString parent_dir;
};

AssertionDialog::AssertionDialog(QWidget *parent, const QList<TElm> & dirList) :
    QDialog(parent),
    ui(new Ui::AssertionDialog),
    m_dirsList(dirList)
{
    ui->setupUi(this);

    //QCommonStyle style;
    //ui->pushButtonReverse->setIcon(style.standardIcon(QStyle::SP_ArrowBack));

    QDateTime minDate =  QDateTime::currentDateTime();
    QDateTime maxDate(QDate(2000, 7, 7), QTime(16, 30, 0));
    foreach (const TElm &row,  m_dirsList)  {
        if (minDate > row.date) {
            minDate = row.date;
        }
        if (maxDate < row.date ) {
            maxDate = row.date;
        }
    }

    ui->labelLeftDate->setText(minDate.toString("dd.MM.yyyy"));
    ui->labelRightDate->setText( maxDate.toString("dd.MM.yyyy"));
    ui->horizontalSlider->setRange(minDate.toTime_t(), maxDate.toTime_t());
    ui->horizontalSlider->setTickInterval(86400);

    m_currentDate = minDate;
}

AssertionDialog::~AssertionDialog()
{
    delete ui;
}

void AssertionDialog::on_buttonBox_accepted()
{
    accept();
}

void AssertionDialog::on_horizontalSlider_valueChanged(int value)
{
    int valChanged = value;
    // Timestamp to datetime
    if (ui->horizontalSlider->invertedAppearance()) {
         valChanged = ui->horizontalSlider->maximum() - value + ui->horizontalSlider->minimum();
     }
     m_currentDate.setTime_t(valChanged);
     m_currentDate.setTime(QTime(0,0,0));
     ui->labelCurrent->setText(m_currentDate.toString("dd.MM.yyyy"));
     calcDirCounts();
}


void AssertionDialog::calcDirCounts()
{
    QList<TDirStatItem> statList {};
    foreach (const TElm &row,  m_dirsList)  {
        QString pathId = row.path;
        if (filterTime(row.date) ) {
            auto itr = std::find_if(statList.begin(), statList.end(), [&pathId](TDirStatItem& someclass) { return someclass.path == pathId; });
             if(itr != statList.end()) {
                 (*itr).inc();
             }
             else {
                statList.append(TDirStatItem(pathId, row.dir, row.parent_dir));
             }
        }
    }

    int i = 0;
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(statList.length());

    foreach (const TDirStatItem &row, statList) {
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText(row.parent_dir);
        item->setData(Qt::UserRole, statList.size() - 1);
        ui->tableWidget->setItem(i, 0, item);

        item = new QTableWidgetItem();
        item->setText(row.dir);
        ui->tableWidget->setItem(i, 1, item);

        int c = row.count;
        item = new QTableWidgetItem();
        //item->setText(QString::number(row.count));
        item->setData(Qt::EditRole, c);
        ui->tableWidget->setItem(i, 2, item);

        i++;
    }
    ui->tableWidget->setSortingEnabled(true);
}

bool AssertionDialog::filterTime(const QDateTime &value)
{
    if (ui->pushButtonReverse->isChecked())
        return (value <= m_currentDate);

    return value >= m_currentDate;
}



void AssertionDialog::on_toolButtonReverse_triggered(QAction *arg1)
{
    //ui->toolButtonReverse->setDown(! ui->toolButtonReverse->isDown());
}

void AssertionDialog::on_pushButtonReverse_clicked()
{
    // ui->pushButtonReverse->setChecked(! ui->pushButtonReverse->isChecked());
    ui->horizontalSlider->setInvertedAppearance(! ui->pushButtonReverse->isChecked());


    if (ui->horizontalSlider->invertedAppearance()) {
         ui->horizontalSlider->setValue(ui->horizontalSlider->maximum() - ui->horizontalSlider->value() + ui->horizontalSlider->minimum());
     }
     else
     {
        ui->horizontalSlider->setValue(m_currentDate.toTime_t());
     }
}
