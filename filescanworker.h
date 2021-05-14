#ifndef FILESCANWORKER_H
#define FILESCANWORKER_H

#include <QObject>

class FileScanWorker : public QObject
{
    Q_OBJECT
public:
    explicit FileScanWorker(QObject *parent = nullptr);
    FileScanWorker(QString base_url, QObject *parent = nullptr);

public slots:
    void beginScan();

signals:

private:
    QString m_baseUrl;

};

#endif // FILESCANWORKER_H
