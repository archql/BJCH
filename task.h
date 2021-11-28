#ifndef TASK_H
#define TASK_H

#include <QObject>
#include "cell.h"

static void _check_task_1(cell *c, qint32 *ctr, qint32 argument)
{
    *ctr += (c->getType() == "Workplace" && c->getNoise() < argument);
}
static void _check_task_2(cell *c, qint32 *ctr, qint32 argument)
{
    *ctr += (c->getType() == "Workplace");
}
static void _check_task_3(cell *c, qint32 *ctr, qint32 argument)
{
    *ctr += (c->getType() == "Wall1" && !c->locked);
}
static void _check_task_4(cell *c, qint32 *ctr, qint32 argument)
{
    *ctr += (c->getType() == "Wall2" && !c->locked);
}
static void _check_task_5(cell *c, qint32 *ctr, qint32 argument)
{
    *ctr += (c->getType() == "Wall3" && !c->locked);
}
static void _check_task_6(cell *c, qint32 *ctr, qint32 argument)
{
    *ctr += (c->getType() == "Wall4" && !c->locked);
}
static void _check_task_7(cell *c, qint32 *ctr, qint32 argument)
{
    *ctr += (c->getType() == "Air" && !c->locked);
}
static void _check_task_8(cell *c, qint32 *ctr, qint32 argument)
{
    if (c->getType() == "Workplace")
    {
        for (cell *n : c->rneibours())
            if (n != nullptr && n->getType() == "Workplace")
                return;
        (*ctr)++;
    }
}
static QHash<cell *, qint16> _visited;
static bool _task_9_hlp(cell *c, qint32 ctr, qint32 argument)
{
    //if (!_visited.contains(c))
        _visited[c] = ctr;
    if (ctr >= argument)
        return false;
    for (cell *n : c->rneibours())
    {
        if (n == nullptr)
            continue;
        if (n->getType() == "Workplace")
            return true;
        if ((!_visited.contains(n) || (_visited[n] > ctr + 1)) && n->getType() == "Air"
                 && _task_9_hlp(n, ctr + 1, argument))
                return true;
    }
    return false;
}
static void _check_task_9(cell *c, qint32 *ctr, qint32 argument)
{
    if (c->getType() == "Emitter")
    {
        _visited.clear();
        *ctr += _task_9_hlp(c, 0, argument);
    }
}
const static QHash<qint8, void(*)(cell *, qint32 *, qint32)> CHECK_FUNCTION =
        {{0x00, _check_task_1},
        {0x10, _check_task_2},
        {0x20, _check_task_3},
        {0x30, _check_task_4},
        {0x40, _check_task_5},
        {0x50, _check_task_6},
        {0x60, _check_task_7},
        {0x70, _check_task_8},
        {0x80, _check_task_9}};

static bool _ifcompleted_1(qint32 ctr, qint32 tgt)
{
    return ctr == tgt;
}
static bool _ifcompleted_2(qint32 ctr, qint32 tgt)
{
    return ctr >= tgt;
}
static bool _ifcompleted_3(qint32 ctr, qint32 tgt)
{
    return ctr <= tgt;
}

const static QHash<qint8, bool(*)(qint32, qint32)> IFCOMPLETED_FUNCTION =
{{0x00, _ifcompleted_1},
{0x01, _ifcompleted_2},
{0x02, _ifcompleted_3}};

const static QHash<qint8, QString> ID_DESCRIPTION_A =
{{0x00, "workplaces has noise level less than"},
{0x10, "use workplaces"},
{0x20, "use walls type 1"},
{0x30, "use walls type 2"},
{0x40, "use walls type 3"},
{0x50, "use walls type 4"},
{0x60, "free space"},
{0x70, "workplace has no neiboring workspaces"},
{0x80, "there is at least 1 path from emitter to workspace with length"}};
const static QHash<qint8, QString> ID_DESCRIPTION_B =
{{0x00, "(exactly)"},
{0x01, "(or more)"},
{0x02, "(or less)"}};

class task : public QObject
{
    Q_OBJECT
public:
    explicit task(QObject *parent = nullptr);
    task(qint8 id, qint32 tgt, qint32 arg = 0, QObject *parent = nullptr);
    ~task();

    qint8 id; // 0xAB, there is A: task id, B: condition id
    qint32 cur, tgt, argument;

    Q_INVOKABLE QString getDescription();
    void reset();
    void check(cell *c);
    Q_INVOKABLE bool completed();


};

#endif // TASK_H
