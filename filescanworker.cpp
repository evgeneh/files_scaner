#include "filescanworker.h"

FileScanWorker::FileScanWorker(QObject *parent) :
    QObject(parent),
    m_baseUrl(QString())
{

}

FileScanWorker::FileScanWorker(QString base_url, QObject *parent):
    QObject(parent),
    m_baseUrl(base_url)
{

}

void FileScanWorker::beginScan()
{

}
