#ifndef CONTROLMODEL_H
#define CONTROLMODEL_H

#include <QAbstractListModel>
#include "cell.h"
#include "cordsystem.h"

class ControlModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ControlModel(QObject *parent = nullptr);

    // General
    Q_INVOKABLE void gen(int widht,int height);

    Q_INVOKABLE void update(int x,int y, int radius, int force);

    void raycast(double x,double y, double vx, double vy, double force);
    Q_INVOKABLE void changeActive1();
    Q_INVOKABLE void changeActive2();
    Q_INVOKABLE void changeActive3();
    Q_INVOKABLE void changeActive4();
    int activeState;

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
        x_role = Qt::UserRole,
        y_role,
        noise_role,
        color_role
    };
    virtual QHash<int, QByteArray> roleNames() const override;

signals:
    void mapReady();

private:
    QList<cell*> cells;
    CordSystem cells_system;
    // TEMP!!!!
};

#endif // CONTROLMODEL_H
