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

#define MIN_GEN_SHOW        0
#define MAX_GEN_CNT         4
#define MIN_GEN_TO_OVERLAP  1
#define FILE_HEADER         ((qint16)(0xF08A))
#define FILE_HEADER_LVL     ((qint16)(0xF082))

static const float STEP = 1.0f;//0.25f;

static const QHash<QString, qint8> CELL_TYPES = {{"Air", 0x55},
                                                 {"Emitter", 0x28},
                                                 {"Workplace", 0x71},
                                                 {"Wall1", 0xA0},
                                                 {"Wall2", 0xA1},
                                                 {"Wall3", 0xA2},
                                                 {"Wall4", 0xA3}};
static const QHash<QString, qint64> TASK_TYPES = {{"task_1", 0xad67a13405fe59b6},
                                                 {"task_2", 0xb6059e913cb6f27b},
                                                 {"task_3", 0xac9514cece0b891a},
                                                 {"task_4", 0x1e086c4690072f05},
                                                 {"task_5", 0x871b445cd661a973},
                                                 {"task_6", 0xdbd5ab1562f6ac0f},
                                                 {"task_7", 0x73a0beed212020be}};

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

    Q_INVOKABLE int getWidth();
    Q_INVOKABLE int getHeight();

    Q_INVOKABLE int update();
    Q_INVOKABLE void resetEmitter(int index, bool ifAdd, int force = -1);

    Q_INVOKABLE bool loadTask(QString taskname);
    Q_INVOKABLE bool parseTasks(QString taskname);
    QQmlListProperty<task>getTasks();
    Q_INVOKABLE void checkCurTasks();
    Q_INVOKABLE QString getTaskDescr(QString taskname);
    Q_INVOKABLE void saveTaskCompletion(QString taskname);
    Q_INVOKABLE void deleteTaskComletion(QString taskname);

    Q_INVOKABLE bool saveToFile(QString filename);
    Q_INVOKABLE bool ldFromFile(QString filename);

    Q_INVOKABLE bool requestAdminKey();
    Q_INVOKABLE bool requestUserId();

    Q_PROPERTY(QQmlListProperty<task> tasks READ getTasks)

    cell* CheckNeibors(float *x, float *y, float vx, float vy, float force, float dst, int gen);

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
        stype_role,
        locked_role
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

    qint64 user_id;
    // TEMP!!!!
};

#endif // CONTROLMODEL_H
