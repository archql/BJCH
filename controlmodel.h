#ifndef CONTROLMODEL_H
#define CONTROLMODEL_H

#include <QAbstractListModel>
#include <QDateTime>
#include <QQueue>
#include <QQmlListProperty>
#include "cell.h"
#include "cordsystem.h"
#include "storage.h"
#include "task.h"

static const qint16 FILE_HEADER = 0xF08A;
static const qint16 FILE_HEADER_LVL = 0xF082;
static const int MAX_GEN_CNT = 2;
static const float STEP = 1.0f;//0.25f;

static const QHash<QString, qint8> CELL_TYPES = {{"Air", 0x55},
                                                 {"Emitter", 0x28},
                                                 {"Workplace", 0x71},
                                                 {"Wall1", 0xA0},
                                                 {"Wall2", 0xA1},
                                                 {"Wall3", 0xA2},
                                                 {"Wall4", 0xA3}};

//typedef struct
//{
//    cell *c;
//    float force;
//    int gen;
//} npacket;

typedef struct
{
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

    Q_INVOKABLE int update();
    Q_INVOKABLE void resetEmitter(int index, bool ifAdd, int force = -1);

    Q_INVOKABLE bool loadTask(QString taskname);
    Q_INVOKABLE bool parseTasks(QString taskname);
    QQmlListProperty<task>getTasks();
    Q_INVOKABLE void checkCurTasks();

    Q_INVOKABLE bool saveToFile(QString filename);
    Q_INVOKABLE bool ldFromFile(QString filename);

    Q_INVOKABLE bool requestAdminKey();

    Q_PROPERTY(QQmlListProperty<task> tasks READ getTasks)

    void CheckNeibors(float *x, float *y, float vx, float vy, float force, float dst, int gen);

    void reset_neibours();
    void raycast(float x,float y, float vx, float vy, float force, float dst, int gen, bool atWall);
    void raycastX(cell *c);
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
    void cellChanged(cell *c);

private:
    QList<task*> tasks;
    QList<cell*> cells;
    QList<cell*> emitters;
    CordSystem cells_system;
    Storage storage;

    QQueue<npacket> queue;
    // TEMP!!!!
};

#endif // CONTROLMODEL_H
