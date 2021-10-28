#include "controlmodel.h"
#include "math.h"
ControlModel::ControlModel(QObject *parent)
    : QAbstractListModel(parent)
{
}


//======================================================
void ControlModel::gen(int width,int height)
{
    activeState = 1;
    cells_system.set(width, height);

    beginResetModel();
    // clear old cells
    cells.clear();
    // add new
    int i, x, y;
    for (i = 0; i < cells_system.maxLinear(); i++)
    {
        cells_system.toBilinear(i, x, y);
        cells << new cell(x, y);
    }

    i = 0;
    for (cell *c : qAsConst(cells))
    {
        // get billinear cords of the point
        cells_system.toBilinear(i, x, y);

        c->setNeibours(cells[cells_system.toLinear(x + 1, y)],
                      cells[cells_system.toLinear(x    , y + 1)],
                      cells[cells_system.toLinear(x - 1, y)],
                      cells[cells_system.toLinear(x    , y - 1)]);

        i++;
    }

    endResetModel();

    emit ControlModel::mapReady();
}
void ControlModel::changeActive1(){
    activeState = 1;
}
void ControlModel::changeActive2(){
    activeState = 2;
}
void ControlModel::changeActive3(){
    activeState = 3;
}
void ControlModel::changeActive4(){
    activeState = 4;
}
void ControlModel::update(int x,int y, int radius, int force)
{   cell *c = cells[cells_system.toLinear(round(x), round(y))];
    if(activeState==4) {
    c->typeOfCell = "Wall3";
    c->color = c->getNoiseColor();
    c->absorb = 45;
    c->reflect = 45;
    }
    else if(activeState==3) {
    c->typeOfCell = "Wall2";
    c->color = c->getNoiseColor();
    c->absorb = 35;
    c->reflect = 35;
    }
    else if(activeState==2) {
    c->typeOfCell = "Wall1";
    c->color = c->getNoiseColor();
    c->absorb = 25;
    c->reflect = 25;
    }
    else if(activeState==1) {
    c->typeOfCell = "Emitter";
    int i = 0, curx, cury;
//    for (cell *c : qAsConst(cells))
//    {
//        cells_system.toBilinear(i, curx, cury);
//        // count noise
//        double tmpn = sqrt((x - curx)*(x - curx) + (y - cury)*(y - cury));
//        qInfo()<<tmpn;
//        if (tmpn < radius)
//            c->noise = force - 1*(tmpn*tmpn); // 5 is constant
//        else
//            c->noise = 0;
//        // get clr
//        c->color = c->getNoiseColor();

//        i++;
//    }
      for (double a = 0; a < 6.28; a += 0.06)
      {
    //double a = 4.0;
        raycast(x, y, sin(a), cos(a), force);
      }
    // temp
    }
    QModelIndex topLeft = createIndex(0,0);
    QModelIndex bottomLeft = createIndex(cells_system.maxLinear(), 0);
    emit dataChanged(topLeft, bottomLeft);

}

void ControlModel::raycast(double x, double y, double vx, double vy, double force)
{
    while (force > 1.) {
        // add force
        cell *c = cells[cells_system.toLinear(round(x), round(y))];
        c->noise = sqrt((c->noise * c->noise) + (force*force));//+= force;
        c->color = c->getNoiseColor();
        // mov point
        x += vx;
        y += vy;
        // dec force
        force -= 3.; // temp value -6db ??
    }

}


//========================================================

int ControlModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return cells.length();
}

QVariant ControlModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    const cell *cur = cells.at(index.row());
    switch (role)
    {
        case x_role: return QVariant(cur->x);
        case y_role: return QVariant(cur->y);
        case noise_role: return QVariant(cur->noise);
        case color_role: return QVariant(cur->color);
    }
    return QVariant();
}

bool ControlModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        cell *cur = cells.at(index.row());
        switch (role)
        {
            case x_role: cur->x = value.toInt();
            case y_role: cur->y = value.toInt();
            case noise_role: cur->noise = value.toDouble();
            case color_role: cur->color = QColor(value.toInt());
        }

        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags ControlModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable; // FIXME: Implement me!
}

bool ControlModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();

    return true;
}

bool ControlModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();

    return true;
}

QHash<int, QByteArray> ControlModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[x_role] = "x";
    names[y_role] = "y";
    names[noise_role] = "noise";
    names[color_role] = "color";
    return names;
}



