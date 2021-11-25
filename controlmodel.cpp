#include "controlmodel.h"
#include "math.h"
ControlModel::ControlModel(QObject *parent)
    : QAbstractListModel(parent)
{
    storage = Storage("nshc", "data");
}


//======================================================
void ControlModel::gen(int width, int height)
{
    //activeState = 1;
    cells_system.set(width, height);

    beginResetModel();
    // clear old cells
    cells.clear();
    // add new
    int i, x, y;
    for (i = 0; i < cells_system.maxLinear(); i++)
    {
        cells_system.toBilinear(i, x, y);
        cells << new cell(x, y, i, this);
    }

    reset_neibours();

    endResetModel();

    emit ControlModel::mapReady();
}
void ControlModel::gen(int width, int height, QVector<QString> cell_types)
{
    //activeState = 1;
    cells_system.set(width, height);

    beginResetModel();
    // clear old cells
    cells.clear();
    // add new
    int i, x, y;
    for (i = 0; i < cells_system.maxLinear(); i++)
    {
        cells_system.toBilinear(i, x, y);
        cells << new cell(x, y, i, cell_types.at(i), this);
    }

    reset_neibours();


    endResetModel();
    emit ControlModel::mapReady();
}
void ControlModel::reset_neibours()
{
    int i = 0, x, y;
    for (cell *c : qAsConst(cells))
    {
        // get billinear cords of the point
        cells_system.toBilinear(i, x, y);
//        int c0 = cells_system.toLinear(x + 1, y);
//        int c1 = cells_system.toLinear(x    , y + 1);
//        int c2 = cells_system.toLinear(x - 1, y);
//        int c3 = cells_system.toLinear(x    , y - 1);
        // this is for simpler vector neibour recognition (order 1-0-2-3)
        /// killed
        ///
        // its 8 neibours
        int c0 = cells_system.toLinear(x + 1, y);
        int c1 = cells_system.toLinear(x + 1, y + 1);
        int c2 = cells_system.toLinear(x    , y + 1);
        int c3 = cells_system.toLinear(x - 1, y + 1);
        int c4 = cells_system.toLinear(x - 1, y);
        int c5 = cells_system.toLinear(x - 1, y - 1);
        int c6 = cells_system.toLinear(x    , y - 1);
        int c7 = cells_system.toLinear(x + 1, y - 1);

        c->setNeibours(c0 != -1 ? cells[c0] : nullptr,
                       c1 != -1 ? cells[c1] : nullptr,
                       c2 != -1 ? cells[c2] : nullptr,
                       c3 != -1 ? cells[c3] : nullptr,
                       c4 != -1 ? cells[c4] : nullptr,
                       c5 != -1 ? cells[c5] : nullptr,
                       c6 != -1 ? cells[c6] : nullptr,
                       c7 != -1 ? cells[c7] : nullptr);
        i++;
    }
}

//void ControlModel::changeActive1(){
//    activeState = 1;
//}
//void ControlModel::changeActive2(){
//    activeState = 2;
//}
//void ControlModel::changeActive3(){
//    activeState = 3;
//}
//void ControlModel::changeActive4(){
//    activeState = 4;
//}

void ControlModel::update(int x,int y, int radius, int force)
{
    QDateTime start = QDateTime::currentDateTime();

//    cell *c = cells[cells_system.toLinear(x, y)];

//        c->setType("Emitter");
//        // emit temp

//        for (cell *c : qAsConst(cells))
//            c->visited.clear();

        // =============================
        // VARIANT OF SPREAD


//        float step = 100. / (float)(force * force);
//        for (float a = 0; a <= 6.29; a += step)
//        {
//            //raycast(x, y, STEP * sin(a), STEP * cos(a), force, 0.f, 0, false);
//            queue.enqueue({});
//        }


        //====================================

        //spread(c, x, y, force, 0);

        // =============================
        // VARIANT OF SPREAD
        // get first cell
//        cell *c = cells.at(cells_system.toLinear(x, y));
//        // post it on queue
//        npacket packet = {c, (float)force, 0};
//        queue.enqueue(packet);

//        while (!queue.isEmpty())
//        {
//            packet = queue.dequeue();
//            c = packet.c;
//            // check if can continue
//            if (c == nullptr)
//                continue;
//            if (packet.gen > MAX_GEN_CNT)
//                continue;

//            // check if wall met
//            // reflect
//            if (c->reflect != -1)
//                queue.enqueue({c, packet.force * c->reflect / 100.f, packet.gen + 1});
//            // absorb
//            if (c->absorb != -1) //TEMP!!!!
//            {
//                packet.force *= (100.f - c->absorb) / 100.f;
//            }
//            // get real force
//            float dst = ((c->x - x) * (c->x - x) + (c->y - y) * (c->y - y)); //Q_rsqrt((float)(x * x + y*y));
//            float real_force = packet.force - 3*log2f(dst) - 11.f;
//            if (real_force < 1.f)
//                continue;

//            // add noise
//            float noise = c->getNoise();
//            noise = 10 * log10f(powf(10, 0.1f * noise) + powf(10, 0.1f * real_force));
//            // TEMP!!!!
//            c->setNoise(noise);

//            if (c->visited.contains(packet.gen))
//                continue;
//            c->visited << packet.gen;

//            // mk 4 packets other
//            QVector<cell *> neibours = c->rneibours();
//            queue.enqueue({neibours[0], packet.force, packet.gen});
//            queue.enqueue({neibours[1], packet.force, packet.gen});
//            queue.enqueue({neibours[2], packet.force, packet.gen});
//            queue.enqueue({neibours[3], packet.force, packet.gen});
//        }


        //==============================

        //float step = 0.005;// 100. / (float)(force * force);
        //float step = 3.14f / 4.f;
        //qInfo() << "Step is " << step;
          //for (float a = 0; a <= 6.29; a += step)
          //{
        //double a = 0.75* 3.14;
            //raycast(x, y, STEP * sin(a), STEP * cos(a), force, 0.f, 0, false);
          //}
        // temp

        //raycastX(c, x, y, force);

    //}


    // update model
    //QModelIndex bottomLeft = createIndex(cells_system.maxLinear(), 0);
    //QModelIndex topLeft = createIndex(0, 0);
    //emit dataChanged(topLeft, bottomLeft);

    qInfo() << "TEMPORALY DISABLED!";
    qInfo() << "UPDATE RUN TOOK: " << start.msecsTo(QDateTime::currentDateTime()) << "ms";

}

void CheckNeibors(float *x, float *y, float vx, float vy) {
    float x1 = *x, x2 = *x+vx, y1 = *y, y2 = *y+vy;
    int xb1 = round(x1), xb2 = round(x2), yb1 = round(y1), yb2 = round(y2);
    if(xb1!=xb2 || yb2!=yb1) {
        int k = (y1-y2)/(x2-x1);
        int b = y1 - k*x1;
        float xang = (float)((xb2+xb1)/2);
        float yang = (float)((yb2+yb1)/2);
        float y3 = k*xang+b;
        if(y3>yang && vx>0 && vy<0) { //то есть если надо идти в точку вверх

        }
        else if(y3<yang && vx>0 && vy<0) { //то есть идём вправо

        }
        else if(y3>yang && vx>0 && vy>0) { //то есть если надо идти в точку вправо

        }
        else if(y3<yang && vx>0 && vy>0) { //то есть если надо идти в точку вниз

        }
        else if(y3>yang && vx<0 && vy>0) { //то есть если надо идти в точку вправо

        }
        else if(y3<yang && vx<0 && vy>0) { //то есть если надо идти в точку вниз

        }
        else if(y3>yang && vx<0 && vy<0) { //то есть если надо идти в точку вправо

        }
        else if(y3<yang && vx<0 && vy<0) { //то есть если надо идти в точку вниз

        }
    }
    else {
        *x = x2;
        *y = y2;

    }
}

//void brezenhamNext(int *x, int *y, float vx, float vy)
//{
//    bool steep = abs(vx) > abs(vy); // Проверяем рост отрезка по оси икс и по оси игрек
//    // Отражаем линию по диагонали, если угол наклона слишком большой
//    if (steep)
//    {
//        swap(x, y); // swap cords if y axis is main
//        swap(&vx, &vy);
//    }
//    // Если линия растёт не слева направо, то меняем начало и конец отрезка местами
//    int error = vx / 2; // Здесь используется оптимизация с умножением на dx, чтобы избавиться от лишних дробей
//    int ystep = (vy >= 0) ? 1 : -1; // Выбираем направление роста координаты y
//    int xstep = (vx >= 0) ? 1 : -1; // Выбираем направление роста координаты y

//    x += xstep;
//    error -= vy;
//    if (error < 0)
//    {
//        y += ystep;
//        error += vx;
//    }

//    if (steep)
//    {
//        swap(x, y); // place cords back
//    }
//}

void ControlModel::spread(cell *c, int sx, int sy, float force, int gen)
{
    if (c == nullptr)
        return;
    if (gen > MAX_GEN_CNT)
        return;
    if (c->visited.contains(gen))
        return;
    float dst = ((c->x - sx) * (c->x - sx) + (c->y - sy) * (c->y - sy)); //Q_rsqrt((float)(x * x + y*y));
    float real_force = force - 3*log2f(dst) - 11.f;
    // check if stop needed
    if (real_force < 1.f)
        return;

    // check if wall met
    if (c->absorb != -1) //TEMP!!!!
    {
        force *= (100.f - c->absorb) / 100.f;
    }

    float noise = c->getNoise();
    noise = 10 * log10f(powf(10, 0.1f * noise) + powf(10, 0.1f * real_force));
    // TEMP!!!!
    c->setNoise(noise);
    c->visited << gen;

    QVector<cell *> neibours = c->rneibours();
    if (neibours[1] != nullptr)
    {
        spread(neibours[1], sx, sy, force, gen);
        if (neibours[1]->reflect != -1)
            spread(neibours[1], sx, sy, force * c->reflect / 100.f, gen + 1);
    }
    if (neibours[0] != nullptr)
    {
        spread(neibours[0], sx, sy, force, gen);
        if (neibours[0]->reflect != -1)
            spread(neibours[0], sx, sy, force * c->reflect / 100.f, gen + 1);
    }

    if (neibours[2] != nullptr)
    {
        spread(neibours[2], sx, sy, force, gen);
        if (neibours[2]->reflect != -1)
            spread(neibours[2], sx, sy, force * c->reflect / 100.f, gen + 1);
    }
    if (neibours[3] != nullptr)
    {
        spread(neibours[3], sx, sy, force, gen);
        if (neibours[3]->reflect != -1)
            spread(neibours[3], sx, sy, force * c->reflect / 100.f, gen + 1);
    }

    // reflections
}

void ControlModel::raycastX(cell *c, float sx, float sy, float force)
{
    // add force to first cell
    float noise = c->getNoise();
    noise = 10 * log10f(powf(10, 0.1f * noise) + powf(10, 0.1f * force));
    // TEMP!!!!
    c->setNoise(noise);
    // set it as visited
    c->visited << 0;

    // - loop for 360 dgs
    // - do:
    //float step = 100. / (float)(force * force);
    float step = 3.14f / 4.f;
    for (float a = 0; a <= 6.29; a += step) // post point to queue with x y vx vy
        queue.enqueue({c, (float)force, 0, sx, sy, STEP * sinf(a), STEP * cosf(a)});

    npacket packet; cell *next;
    while (!queue.isEmpty())
    {
        packet = queue.dequeue();
        c = packet.c;
        if (c == nullptr)
            continue;
        if (packet.gen > MAX_GEN_CNT)
            continue;


        if ((packet.x - c->x) > 0.5 || (packet.y - c->y) > 0.5) // there is an error
        {
            // next?
            // reflect?
        }
            // get next cell
            float fl_x = packet.x - c->x;
            float fl_y = packet.y - c->y;
            int num = 2*((packet.vx + fl_x + packet.vy + fl_y) < 0) + ((-packet.vx - fl_x + packet.vy + fl_y) < 0);
            next = c->rneibours()[num];
            // check if neibour_next <> real_next
            if (abs(c->x + packet.vx - next->x) > 0.5f || abs(c->x + packet.vx - next->x) > 0.5f)

            if (abs(fl_x) > 0.5 || abs(fl_y) > 0.5)
            {
                quick_exit(-1);
            }

            //check if wall met
            // reflect
            if (next != nullptr && next->reflect != -1)
            {
                int tmp;
                if ((num ^ 3) == 0) // reflect v by X axis
                {
                    tmp = cells_system.toLinear(round(c->x - fl_x), c->y);
                    queue.enqueue({tmp != -1 ? cells.at(tmp) : nullptr, (float)force * c->reflect / 100.f, packet.gen + 1,
                                   c->x - fl_x, packet.y, packet.vx, -packet.vy});
                }
                else                // reflect v by Y axis
                {
                    tmp = cells_system.toLinear(c->x, round(c->y - fl_y));
                    queue.enqueue({tmp != -1 ? cells.at(tmp) : nullptr, (float)force * c->reflect / 100.f, packet.gen + 1,
                                   packet.x, c->y - fl_y, -packet.vx, packet.vy});
                }
            }

            // absorb
            if (c->absorb != -1) //TEMP!!!!
            {
                packet.force *= (100.f - c->absorb) / 100.f;
            }
            // get real force
            float dst = ((c->x - sx) * (c->x - sx) + (c->y - sy) * (c->y - sy)); //Q_rsqrt((float)(x * x + y*y));
            float real_force = packet.force - 3*log2f(dst) - 11.f;
            if (real_force < 1.f)
                continue;

            // apply force
            if (!c->visited.contains(packet.gen) && packet.gen >= 1)
            {
                float noise = c->getNoise();
                noise = 10 * log10f(powf(10, 0.1f * noise) + powf(10, 0.1f * real_force));
                // TEMP!!!!
                c->setNoise(noise);
                c->visited << packet.gen;
            }


        // post next point to queue (with own x, y, vx, vy, force, gen)
        packet.x += packet.vx;
        packet.y += packet.vy;
        queue.enqueue({next, packet.force, packet.gen,
                       packet.x, packet.y, packet.vx, packet.vy});
    }


}


void ControlModel::raycastY(cell *c, float force)
{
    // add force to first cell
    float noise = c->getNoise();
    noise = 10 * log10f(powf(10, 0.1f * noise) + powf(10, 0.1f * force));
    // TEMP!!!!
    c->setNoise(noise);
    // set it as visited
    c->visited << 0;

    // - loop for 360 dgs
    // - do:
    float step = 100. / (float)(force * force);
    for (float a = 0; a <= 6.29; a += step) // post point to queue with x y vx vy
        queue.enqueue({c, (float)force, 0, (float)c->x, (float)c->y, STEP * sinf(a), STEP * cosf(a)});

    npacket packet; cell *next, *cur, *reflto;
    while (!queue.isEmpty())
    {
        packet = queue.dequeue();
        if (packet.gen > MAX_GEN_CNT)
            continue;

        cur = packet.c;
        if (cur == nullptr)
            continue;

        // get next cell
        float next_x = packet.x + packet.vx;
        float next_y = packet.y + packet.vy;
        int nextCID = cells_system.toLinear(round(next_x), round(next_y));
        if (nextCID != -1)
        {
            next = cells[nextCID];
            // reflect
            if (/*!packet.atWall &&*/ next->reflect != -1)
            {
                float flx = next_x - round(next_x);
                float fly = next_y - round(next_y);
                // get cell to reflect
                //(flx - vx, fly - vy)
                int num =  num = 2*((-packet.vx + flx - packet.vy + fly) < 0) +
                        ((packet.vx - flx -packet.vy + fly) < 0);
                reflto = c->rneibours()[num];
                if (reflto != nullptr)
                {
                    // check if there is a wall
                    if (reflto->reflect != -1)
                    { // reflect from reflto

                    }
                    else
                    { // reflect from next
                        int tmp;
                        if ((num ^ 3) == 0) // reflect v by X axis
                        {
                            tmp = cells_system.toLinear(round(cur->x - flx), cur->y);
                        }
                        else                // reflect v by Y axis
                        {
                            tmp = cells_system.toLinear(cur->x, round(cur->y - fly));
                        }
                    }
                }

//                queue.enqueue({tmp != -1 ? cells.at(tmp) : nullptr, (float)force * c->reflect / 100.f, packet.gen + 1,
//                               cur->x - fl_x, packet.y, packet.vx, -packet.vy});
//                queue.enqueue({tmp != -1 ? cells.at(tmp) : nullptr, (float)force * c->reflect / 100.f, packet.gen + 1,
//                               packet.x, cur->y - fl_y, -packet.vx, packet.vy});
            }
        }
        else
        {
            next = nullptr;
        }

//            float fl_x = packet.x - c->x;
//            float fl_y = packet.y - c->y;
//            int num = 2*((packet.vx + fl_x + packet.vy + fl_y) < 0) + ((-packet.vx - fl_x + packet.vy + fl_y) < 0);
//            next = c->rneibours()[num];
            // check if neibour_next <> real_next

            // absorb
            if (cur->absorb != -1) //TEMP!!!!
            {
                packet.force *= (100.f - cur->absorb) / 100.f;
            }
            // get real force
            float dst = ((cur->x - c->x) * (cur->x - c->x) + (cur->y - c->y) * (cur->y - c->y)); //Q_rsqrt((float)(x * x + y*y));
            float real_force = packet.force - 3*log2f(dst) - 11.f;
            if (real_force < 1.f)
                continue;

            // apply force
            if (!c->visited.contains(packet.gen) && packet.gen >= 1)
            {
                float noise = cur->getNoise();
                noise = 10 * log10f(powf(10, 0.1f * noise) + powf(10, 0.1f * real_force));
                // TEMP!!!!
                cur->setNoise(noise);
                cur->visited << packet.gen;
            }


        // post next point to queue (with own x, y, vx, vy, force, gen)
        if (next == nullptr)
            continue;
        packet.x = next_x;
        packet.y = next_y;
        queue.enqueue({next, packet.force, packet.gen,
                       packet.x, packet.y, packet.vx, packet.vy});
    }
}


void ControlModel::raycast(float x, float y, float vx, float vy, float force, float dst, int gen, bool atWall)
{
    if (!cells_system.atSystem((int)round(x), (int)round(y)))
        return;
    if (gen > MAX_GEN_CNT)
        return;
    float real_force = force;
    if (dst > 0.1f)
        real_force = force - 6*log2f(dst) - 11.f;
    int max_x, max_y;

    while (real_force > 1.f)
    {
        // get cur cell
        cell *c = cells[cells_system.toLinear(round(x), round(y))];
        // if we hit a wall
        if (c->reflect != -1 && !atWall) //TEMP!!!!
        {
            float fl_x = x - round(x);
            float fl_y = y - round(y);
            float sgn = (vx - fl_x + vy - fl_y) * (-vx + fl_x + vy - fl_y); // idea is get sgn of dot product between sum of speed and (fl_x, fl_y) vectors and different axiles vectors (x and y)
            if (sgn >= 0.f) // Y axis
                raycast( round(x) + vx - fl_x,  y, vx, -vy, force * c->reflect / 100.f, dst + STEP, gen + 1, true);
            if (sgn <= 0.f) // X axis
                raycast( x,  round(y) + vy - fl_y, -vx, vy, force * c->reflect / 100.f, dst + STEP, gen + 1, true);
        }
        atWall = c->absorb != -1;
        if (atWall) // TEMP!!!!
        {
            //qInfo() << "hit a wall ";
            force *= (100.f - c->absorb) / 100.f;
        }
        // if we went out of field
        if (!cells_system.atSystem(x, y)) // point
        {
            break; // kill
            //qInfo() << "went out of field ";
            cells_system.get(max_x, max_y);
            if (x + vx < 0 || x + vx >= max_x)
                vx = -vx;
            if (y + vy < 0 || y + vy >= max_y)
                vy = -vy;
            raycast(x + vx, y + vy, vx, vy, force, dst + STEP, gen + 1, false);
            break; // kill cur ray
        }
        // add force to cell
        if ((!c->visited.contains(gen) || gen != 0) && gen >= 0)
        {
            //qInfo() << "cur: X "<< x << " Y " << y<< " vx " << vx<< " vy " << vy<< " rf " << real_force<< " gen " << gen << " cell " << c->getType() << " dst " << dst;
            float noise = c->getNoise();
            noise = 10 * log10f(powf(10, 0.1f * noise) + powf(10, 0.1f * real_force));//sqrt((c->noise * c->noise) + (force*force));//+= force;
            // TEMP!!!!
            c->setNoise(noise);
            // TEMP!!!!
            c->visited << gen;
        }
        // mov point
        x += vx;
        y += vy;
        dst += STEP;
        real_force = force - 6*log2f(dst) - 11.f;
        // dec force
        //force -= 3.; // temp value -6db ??
    }
    //qInfo() << "END RAYCAST: low force";

}


bool ControlModel::saveToFile(QString filename)
{
    // get cur fld sz
    int width, height;
    cells_system.get(width, height);

    QVector<QString> data = QVector<QString>();
    // save cur sz
    data.append(QString::number(width));
    data.append(QString::number(height));
    // save points
    for (const cell *c : qAsConst(cells))
    {
        data.append(c->getType());
    }
    return storage.saveToFile(filename, data);
}
bool ControlModel::ldFromFile(QString filename)
{
    // get data
    QVector<QString> data = storage.loadFromFile(filename);
    if (data.isEmpty())
        return false;

    // get fld sz
    int width = data.takeFirst().toInt();
    int height = data.takeFirst().toInt();
    gen(width, height, data);

    return true;
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
        case noise_role: return QVariant(cur->getNoise());
        case color_role: return QVariant(cur->color);
        case wstate_role: return QVariant(cur->wallstate);
        case stype_role: return QVariant(cur->getType());
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
        case x_role: cur->x = value.toInt(); break;
        case y_role: cur->y = value.toInt(); break;
        case noise_role: cur->setNoise(value.toFloat()); break;
        case color_role: cur->color = QColor(value.toInt()); break;
        case wstate_role: cur->wallstate = value.toInt(); break;
        case stype_role: cur->setType(value.toString()); break;
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
    names[wstate_role] = "wstate";
    names[stype_role] = "typeOfCell";
    return names;
}



