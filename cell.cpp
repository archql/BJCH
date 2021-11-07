#include "cell.h"

cell::cell(QObject *parent) : QObject(parent)
{
    Neibours.fill(nullptr, 4);
}

cell::cell(int x, int y, QObject *parent) : QObject(parent)
{
    Neibours.fill(nullptr, 4);

    this->x = x;
    this->y = y;
    noise = 0.f;
    setType("Air");
    color = getNoiseColor();
}

cell::cell(int x, int y, QString cellType, QObject *parent) : QObject(parent)
{
    Neibours.fill(nullptr, 4);

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
        reflect = 70;
    } else {
        absorb = -1;
        reflect = -1;
    }
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
void cell::setNeibours(cell *p0, cell *p1, cell *p2, cell *p3)
{
    Neibours[0] = p0;
    Neibours[1] = p1;
    Neibours[2] = p2;
    Neibours[3] = p3;
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
