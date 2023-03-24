#ifndef FILESCANWORKER_H
#define FILESCANWORKER_H

#include <QObject>
#include <QDateTime>

struct TElm {
    TElm():
        id(-1)
    {}

    TElm(long _id, const QString& _name, const QDateTime& _date, const QDateTime& _last_read, const QString& _path, const QString& _dir, const QString& _parent, qint64 _size):
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

struct TDirStat {
    TDirStat():
        parent(QString()), name(QString()), elmCount(0), totalSize(0), minDate(QDateTime()), maxDate(QDateTime()),
        oldestFileName(QString()), latestFileName(QString())
    {

    }
    TDirStat(TElm elm): elmCount(1), oldestFileName(QString()), latestFileName(QString()) {
        parent = elm.parent_dir;
        name = elm.dir;
        totalSize = elm.size;
        minDate = elm.date;
        maxDate = elm.date;
    }
    void updateDir(TElm elm) {
        elmCount += 1;
        totalSize += elm.size;
        if (elm.date > maxDate){
            maxDate = elm.date;
            latestFileName = elm.name;
        }
        else if (elm.date < minDate){
            minDate = elm.date;
            oldestFileName = elm.name;
        }
    }
    QString parent;
    QString name;    
    long totalSize;
    int elmCount;
    QDateTime minDate;
    QDateTime maxDate;
    QString oldestFileName;
    QString latestFileName;
};

class FileScanWorker : public QObject
{
    Q_OBJECT
public:
    explicit FileScanWorker(QObject *parent = nullptr);

    FileScanWorker(QString base_url, const QList<QString> &extensions, QObject *parent = nullptr);

    QList<TElm> dirList() const;

public slots:
    void beginScan();

signals:
    void finish();
private:
    QString m_baseUrl;
    QList<TElm> m_dirsList;
    long m_dirCount;
    long m_fileCount;
    QList<QString> m_extensions;

    void scanDir(const QString &dir_path, const QString &parent);

};

#endif // FILESCANWORKER_H
