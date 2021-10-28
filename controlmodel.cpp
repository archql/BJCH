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
{
    cell *c = cells[cells_system.toLinear(round(x), round(y))];
    if (activeState==4)
    {
        c->typeOfCell = "Wall3";
        c->color = c->getNoiseColor();
        c->absorb = 45;
        c->reflect = 45;
    }
    else if(activeState==3)
    {
        c->typeOfCell = "Wall2";
        c->color = c->getNoiseColor();
        c->absorb = 35;
        c->reflect = 35;
    }
    else if(activeState==2)
    {
        c->typeOfCell = "Wall1";
        c->color = c->getNoiseColor();
        c->absorb = 25;
        c->reflect = 25;
    }
    else if (activeState == 1 && (force != 0))
    {
        c->typeOfCell = "Emitter";
        // emit temp

        for (cell *c : qAsConst(cells))
            c->visited.clear();

        float step = 100. / (float)(force * force);
        qInfo() << "Step is " << step;
          for (float a = 0; a < 6.28; a += step)
          {
        //double a = 4.0;
            raycast(x, y, sin(a), cos(a), force, 0.f, 0);
          }
        // temp

    }
    QModelIndex bottomLeft = createIndex(cells_system.maxLinear(), 0);
    QModelIndex topLeft = createIndex(0, 0);
    emit dataChanged(topLeft, bottomLeft);

}

void ControlModel::raycast(float x, float y, float vx, float vy, float force, float dst, int gen)
{
    if (gen > 6 /*|| !cells_system.atSystem(x + vx, y + vy)*/)
        return;
    float real_force = force;
    int max_x, max_y;

    while (real_force > 1.f) {
        // add force
        cell *c = cells[cells_system.toLinear(round(x), round(y))];
        if (!c->visited.contains(gen))
        {
            c->noise = 10 * log10f(powf(10, 0.1f * c->noise) + powf(10, 0.1f * real_force));//sqrt((c->noise * c->noise) + (force*force));//+= force;
            c->color = c->getNoiseColor();
            c->visited << gen;
        }
        // if we went out of field
        if (!cells_system.atSystem(x + vx, y + vy)) // point
        {
            cells_system.get(max_x, max_y);
            if (x + vx < 0 || x + vx >= max_x)
                vx = -vx;
            if (y + vy < 0 || y + vy >= max_y)
                vy = -vy;
            raycast(x, y, vx, vy, real_force, dst + 1.f, gen + 1);
            break; // kill cur ray
        }
        // mov point
        x += vx;
        y += vy;
        dst += 1.f;
        real_force = force - 6*log2f(dst) - 11.f;
        // dec force
        //force -= 3.; // temp value -6db ??
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



