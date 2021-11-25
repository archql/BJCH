#ifndef CONTROLMODEL_H
#define CONTROLMODEL_H

#include <QAbstractListModel>
#include <QDateTime>
#include <QQueue>
#include "cell.h"
#include "cordsystem.h"
#include "storage.h"

static const int MAX_GEN_CNT = 1;
static const float STEP = 0.25f;

//typedef struct
//{
//    cell *c;
//    float force;
//    int gen;
//} npacket;

typedef struct
{
    cell *c;
    float force;
    int gen;
    float x, y, vx, vy;
} npacket;

static float Q_rsqrt( float number )
{
    const float x2 = number * 0.5F;
    const float threehalfs = 1.5F;

    union {
        float f;
        uint32_t i;
    } conv = {number}; // member 'f' set to value of 'number'.
    conv.i = 0x5f3759df - ( conv.i >> 1 );
    conv.f *= threehalfs - x2 * conv.f * conv.f;
    return conv.f;
}

class ControlModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ControlModel(QObject *parent = nullptr);

    // General
    Q_INVOKABLE void gen(int widht, int height);
    Q_INVOKABLE void gen(int widht, int height, QVector<QString> cell_types);

    Q_INVOKABLE void update(int x,int y, int radius, int force);

    Q_INVOKABLE bool saveToFile(QString filename);
    Q_INVOKABLE bool ldFromFile(QString filename);

//    Q_INVOKABLE void changeActive1();
//    Q_INVOKABLE void changeActive2();
//    Q_INVOKABLE void changeActive3();
//    Q_INVOKABLE void changeActive4();
//    int activeState;

    void reset_neibours();
    void raycast(float x,float y, float vx, float vy, float force, float dst, int gen, bool atWall);
    void raycastX(cell *c, float sx, float sy, float force);
    void raycastY(cell *cd, float force);

    void spread(cell *c, int sx, int sy, float force, int gen);

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
        color_role,
        wstate_role,
        stype_role
    };
    virtual QHash<int, QByteArray> roleNames() const override;

signals:
    void mapReady();
    void cellChanged(int index);
public slots:
    void receiveCellChange(const int x, const int y);

private:
    QList<cell*> cells;
    CordSystem cells_system;
    Storage storage;

    QQueue<npacket> queue;
    // TEMP!!!!
};

#endif // CONTROLMODEL_H
