#include "filescanworker.h"
#include <QDir>
#include <QFileInfoList>

FileScanWorker::FileScanWorker(QObject *parent) :
    QObject(parent),
    m_baseUrl(QString())
{

}

FileScanWorker::FileScanWorker(QString base_url, const QList<QString> &extensions, QObject *parent):
    QObject(parent),
    m_baseUrl(base_url),
    m_dirCount(0),
    m_fileCount(0),
    m_extensions(extensions)
{

}

QList<TElm> FileScanWorker::dirList() const
{
    return m_dirsList;
}

void FileScanWorker::beginScan()
{
    m_dirsList.clear();

    scanDir(m_baseUrl, "__ROOT__");

    emit finish();
}


void FileScanWorker::scanDir(const QString &dir_path, const QString &parent)
{
    QDir directory(dir_path);
    QFileInfoList dirItemsVector;
    if (!directory.exists()){
        return;
    }
    dirItemsVector = directory.entryInfoList();

    if (dirItemsVector.size() > 2)
    {
        QFileInfoList::iterator dir_item;
        for (dir_item = dirItemsVector.begin(); dir_item != dirItemsVector.end(); ++dir_item) {
            if (dir_item->fileName() != "." && dir_item->fileName() != ".." ) {
                if (dir_item->isDir()) {
                    // parse as dir
                    if (! dir_item->fileName().contains("_files"))
                    {
                        m_dirCount++;
                        scanDir(dir_item->filePath(), directory.dirName());
                    }
                } else if (dir_item->isFile()){
                    //parse as file
                    if (!m_extensions.contains(dir_item->completeSuffix().toLower()))
                    {
                        continue;
                    }

                    QDateTime d_created = dir_item->created();
                    QDateTime d_last_read = dir_item->lastRead();
                    QString dir =  directory.dirName().section('\\', -1);
                    m_fileCount++;
                    TElm elm{m_fileCount, dir_item->fileName(), d_created, d_last_read, dir_item->path(), dir, parent, dir_item->size()};
                    m_dirsList << elm;
                }

            }
        }
    }
}
