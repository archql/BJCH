#include "cell.h"
#include "controlmodel.h"

cell::cell(QObject *parent) : QObject(parent)
{    
    Neibours.fill(nullptr, 8);
    setType("Air");
}
cell::cell(int x, int y, int index, QObject *parent) : QObject(parent)
{
    Neibours.fill(nullptr, 8);

    locked = false;
    wallstate = 0;
    force = -1;
    this->x = x;
    this->y = y;
    this->index = index;
    noise = 0.f;
    setType("Air");
    updateNoiseColor();
}

cell::cell(int x, int y, int index, QString cellType, bool locked, QObject *parent) : QObject(parent)
{
    Neibours.fill(nullptr, 8);

    this->locked = locked;
    wallstate = 0;
    force = -1;
    this->x = x;
    this->y = y;
    this->index = index;
    noise = 0.f;
    setType(cellType);
    updateNoiseColor();
}

cell::~cell()
{

}

void cell::setNoise(const float noise)
{
    this->noise = noise;
    updateNoiseColor();
}

void cell::forceSetNoise(const float noise)
{
    this->noise = noise;
}
void cell::setType(const QString cellType)
{
    typeOfCell = cellType;
    //color = getNoiseColor();
    // do some processing
    // IF NEW ELEMENT IS EMITTER YOU NEED TO CALL RESET EMITTER FROM MODEL!
    if (typeOfCell == "Wall1") {
        absorb = 5;
        reflect = 95;
    } else if (typeOfCell == "Wall2") {
        absorb = 15;
        reflect = 20;
    } else if (typeOfCell == "Wall3") {
        absorb = 25;
        reflect = 80;
    } else if (typeOfCell == "Wall4") {
        absorb = 40;
        reflect = 100; //90
    } else {
        absorb = -1;
        reflect = -1;
    }

    if ((absorb != -1) != (wallstate & 1))
    {
        int i = 0;
        for (cell *c : Neibours)
        {
            if (c != nullptr)
                c->checkWallstate((i + 4) % 8);
            i++;
        }
        wallstate ^= 1;
    }
    // temporaly
    auto model = dynamic_cast<ControlModel*>(parent());
    if (model)
    {
        emit model->cellChanged(this);
    }
}

void cell::checkWallstate(const int index)
{
    wallstate ^= 2 << index; // do if wallstate changed

    // temporaly
    auto model = dynamic_cast<ControlModel*>(parent());
    if (model)
    {
        emit model->cellChanged(this);
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
void cell::updateNoiseColor()
{
    //if(typeOfCell == "Wall1") return QColor("lightGray");
    //if(typeOfCell == "Wall2") return QColor("darkGray");
    //if(typeOfCell == "Wall3") return QColor("gray");
    //if(typeOfCell == "Wall4") return QColor("gray");
    //if(typeOfCell == "Emitter") return QColor("black");
    if(noise <= 5)
        color = QColor("skyblue");
    else if(noise <=10)
        color =  QColor("mediumaquamarine");
    else if(noise <=20)
        color =  QColor("mediumseagreen");
    else if(noise <=40)
        color =  QColor("lightgreen");
    else if(noise <=60)
        color =  QColor("khaki");
    else if(noise <=80)
        color =  QColor("orange");
    else if(noise <=110)
        color =  QColor("darkorange");
    else if(noise <=130)
        color =  QColor("lightcoral");
    else if(noise <=150)
        color =  QColor("palevioletred");
    else if(noise <=170)
        color =  QColor("indianred");
    else if(noise <=190)
        color =  QColor("firebrick");
    else
        color =  QColor("maroon");
}
