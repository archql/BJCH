#include "cell.h"

cell::cell(QObject *parent) : QObject(parent)
{
    Neibours.fill(nullptr, 8);
    setType("Air");

    connect(this, SIGNAL(selfChanged(int,int)), parent, SLOT(receiveCellChange(int,int)));
}
cell::cell(int x, int y, QObject *parent) : QObject(parent)
{
    connect(this, SIGNAL(selfChanged(int,int)), parent, SLOT(receiveCellChange(int,int)));

    Neibours.fill(nullptr, 8);

    wallstate = 0;
    this->x = x;
    this->y = y;
    noise = 0.f;
    setType("Air");
    color = getNoiseColor();
}

cell::cell(int x, int y, QString cellType, QObject *parent) : QObject(parent)
{
    connect(this, SIGNAL(selfChanged(int,int)), parent, SLOT(receiveCellChange(int,int)));

    Neibours.fill(nullptr, 8);

    wallstate = 0;
    this->x = x;
    this->y = y;
    noise = 0.f;
    setType(cellType);
    color = getNoiseColor();
}

cell::~cell()
{

}

void cell::setNoise(const float noise)
{
    this->noise = noise;
    color = getNoiseColor();
}
void cell::setType(const QString cellType)
{
    typeOfCell = cellType;
    color = getNoiseColor();
    // do some processing
    if (typeOfCell == "Wall1") {
        absorb = 10;
        reflect = 90;
    } else if (typeOfCell == "Wall2") {
        absorb = 20;
        reflect = 80;
    } else if (typeOfCell == "Wall3") {
        absorb = 30;
        reflect = 100; //90
    } else {
        absorb = -1;
        reflect = -1;
    }

    if ((absorb != -1) != (wallstate & 1))
    {
        for (cell *c : Neibours)
            c->checkWallstate(); // do it not for all -- only for changed
        wallstate ^= 1;
    }
    qInfo() << "Wallstate check!";
    emit selfChanged(x, y);
}

void cell::checkWallstate()
{
    int wallstate_old = wallstate;
    int mask = 1; // zero bit is for self-walled check
    wallstate &= 1;
    for (cell *c : Neibours)
    {
        mask <<= 1;
        if (c == nullptr)
            continue;
        if ((c->absorb != -1) != !!(wallstate_old & mask))
        {
            wallstate ^= mask; // do if wallstate changed
        }
    }
    emit selfChanged(x, y);
}
float cell::getNoise() const
{
    return noise;
}
QString cell::getType() const
{
    return typeOfCell;
}

QVector<cell *> &cell::rneibours()
{
    return Neibours;
}
void cell::setNeibours(cell *p0, cell *p1, cell *p2, cell *p3, cell *p4, cell *p5, cell *p6, cell *p7)
{
    Neibours[0] = p0;
    Neibours[1] = p1;
    Neibours[2] = p2;
    Neibours[3] = p3;
    Neibours[4] = p4;
    Neibours[5] = p5;
    Neibours[6] = p6;
    Neibours[7] = p7;
}

// static
QColor cell::getNoiseColor()
{
    if(typeOfCell == "Wall1") return QColor("lightGray");
    if(typeOfCell == "Wall2") return QColor("darkGray");
    if(typeOfCell == "Wall3") return QColor("gray");
    if(typeOfCell == "Emitter") return QColor("black");
    if(noise <= 5)
        return QColor("skyblue");
    else if(noise <=10)
        return QColor("mediumaquamarine");
    else if(noise <=20)
        return QColor("mediumseagreen");
    else if(noise <=40)
        return QColor("lightgreen");
    else if(noise <=60)
        return QColor("khaki");
    else if(noise <=80)
        return QColor("orange");
    else if(noise <=110)
        return QColor("darkorange");
    else if(noise <=130)
        return QColor("lightcoral");
    else if(noise <=150)
        return QColor("palevioletred");
    else if(noise <=170)
        return QColor("indianred");
    else if(noise <=190)
        return QColor("firebrick");
    else
        return QColor("maroon");
}
