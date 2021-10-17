#include "controlmodel.h"

ControlModel::ControlModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

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
        case StartX_Role: return QVariant(cur->start_x);
        case StartY_Role: return QVariant(cur->start_y);
    }
    return QVariant();
}

bool ControlModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
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
    names[StartX_Role] = "start_x";
    names[StartY_Role] = "start_y";
    return names;
}

//======================================================
void ControlModel::gen(int x_range,int y_range, int generations)
{
    int sx=getRandomInt(0,x_range);
    int sy=-10;
    int ex=getRandomInt(-100,100)+sx;
    int ey=sy+y_range;

    double maxOffcet = 0.1;

    QList<cell*> generated;
    generated.push_back(new cell(sx,sy,ex,ey,5, this));

    for (int i = 0; i < generations; i++)
    {
        QList<cell*> local;

        for (auto cur : generated)
        {
            int sx = cur->start_x;
            int sy = cur->start_y;

            int ex = cur->end_x;
            int ey = cur->end_y;

            //find middle point
            int dx = ex - sx;
            int dy = ey - sy;

            double choose = getRandom() * 0.2 + 0.4;
            int midX = int(dx * choose + sx);
            int midY = int(dy * choose + sy);

            //perpendicular
            if (getRandom() > 0.5)
                maxOffcet = -maxOffcet;

            double lineCoeff = getRandom() * 2 * maxOffcet - maxOffcet;
            lineCoeff = maxOffcet;

            int px = int(lineCoeff * dy * -1);
            int py = int(lineCoeff * dx);

            midX += px;
            midY += py;

            //fork form
            int dirX = midX - sx;
            int dirY = midY - sy;

            //add new segments

            local.push_back(new cell(sx, sy, midX, midY, cur->size, this));
            local.push_back(new cell(midX, midY, ex, ey, cur->size, this));

            double r = getRandom();
            if (r > 0.4)
            {
                local.push_back(new cell(midX, midY, midX + dirX, midY + dirY,
                                            cur->size / 2, this));
            }

            delete cur;
        }
        generated.clear();

        generated = local;
    }
    beginResetModel();
    cells.clear();
    cells = generated;
    endResetModel();

    emit ControlModel::mapReady();
}


//========================================================
//===========Counters================
double ControlModel::countAverage(double data[],int data_size)
{
    double sum=0.;
    for(int i=0;i<data_size;i++){
        sum+=data[i];
    }
    return sum/data_size;
}

int ControlModel::getRandomInt(int min,int max)
{

    return (qrand() % (max-min)) + min;

}
double ControlModel::getRandom()
{
    return double(getRandomInt(0,10000))/double(10000);
}

