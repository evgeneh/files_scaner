#ifndef FILESCANWORKER_H
#define FILESCANWORKER_H

#include <QObject>
#include <QDateTime>

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
