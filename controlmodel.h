#ifndef CONTROLMODEL_H
#define CONTROLMODEL_H

#include <QAbstractListModel>
#include "cell.h"

class ControlModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ControlModel(QObject *parent = nullptr);

    // General
    Q_INVOKABLE void gen(int x_range,int y_range, int generations);





    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // There is roles of each property element has
    enum
    {
        StartX_Role = Qt::UserRole,
        StartY_Role
    };
    virtual QHash<int, QByteArray> roleNames() const override;

signals:
    void mapReady();

private:
    QList<cell*> cells;
    // TEMP!!!!
    double countAverage(double data[],int data_size);

    int getRandomInt(int min,int max);
    double getRandom();
};

#endif // CONTROLMODEL_H
