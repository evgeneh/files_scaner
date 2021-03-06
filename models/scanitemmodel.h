#ifndef SCANITEMMODEL_H
#define SCANITEMMODEL_H

#include <QAbstractTableModel>

class ScanItemModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ScanItemModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
};

#endif // SCANITEMMODEL_H
