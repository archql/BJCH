#include "task.h"

task::task(QObject *parent) : QObject(parent)
{

}

task::task(qint8 id, qint32 tgt, qint32 arg, QObject *parent) : QObject(parent)
{
    this->id = id;
    cur = 0;
    this->tgt = tgt;
    argument = arg;
}

task::~task()
{

}

QString task::getDescription()
{
    return ID_DESCRIPTION_A[id & 0xF0] + (argument ?  " " + QString::number(argument) : "") + " "
            + QString::number(cur) + "/" + QString::number(tgt) + " " + ID_DESCRIPTION_B[id & 0x0F];
}

void task::reset()
{
    cur = 0;
}

void task::check(cell *c)
{
    CHECK_FUNCTION[id & 0xF0](c, &cur, argument);
}

bool task::completed()
{
    return IFCOMPLETED_FUNCTION[id & 0x0F](cur, tgt);
}
