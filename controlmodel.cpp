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
    cells_system.set(width, height);

    beginResetModel();
    // clear old cells
    cells.clear();
    emitters.clear();
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
    qInfo()<<"DISABLED!";
//    //activeState = 1;
//    cells_system.set(width, height);
//    emit ControlModel::mapReady(); // temp here to update view

//    qInfo("beginResetModel");
//    beginResetModel();
//    // clear old cells
//    cells.clear();
//    emitters.clear();
//    // add new
//    int i = 0, x, y;
//    for (QString celltype : cell_types)
//    {
//        cells_system.toBilinear(i, x, y);
//        cells << new cell(x, y, i, celltype, this);
//        if (celltype == "Emitter")
//            emitters << cells.last();
//        i++;
//    }


//    reset_neibours();
//    // update cells wallstate
//    for (cell *cur : cells)
//        if (cur->wallstate & 1)
//        {
//            i = 0;
//            for (cell *c : cur->rneibours())
//            {
//                if (c != nullptr)
//                    c->checkWallstate((i + 4) % 8); // do it not for all -- only for changed
//                i++;
//            }
//        }

//    qInfo("endResetModel");
//    endResetModel();
}
void ControlModel::reset_neibours()
{
    int i = 0, x, y;
    for (cell *c : qAsConst(cells))
    {
        // get billinear cords of the point
        cells_system.toBilinear(i, x, y);
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

int ControlModel::update()
{
    QDateTime start = QDateTime::currentDateTime();

    for (cell *c : qAsConst(cells))
        c->setNoise(0.f); // forceSet !! !!

        // =============================
        // VARIANT OF SPREAD
     for (cell* emitter : emitters)
     {
         for (cell *c : qAsConst(cells))
             c->visited.clear();
//         float step = 20. / (float)(emitter->force * emitter->force);
//         for (float a = 0; a <= 6.29; a += step)
//         {
//             raycast(emitter->x, emitter->y, STEP * sin(a), STEP * cos(a), emitter->force, 0.f, 0, false);
//             //queue.enqueue({});
//         }
         raycastX(emitter);
     }

    // update model
    QModelIndex bottomLeft = createIndex(cells_system.maxLinear(), 0);
    QModelIndex topLeft = createIndex(0, 0);
    emit dataChanged(topLeft, bottomLeft);

    //qInfo() << "TEMPORALY DISABLED!";
    qInfo() << "UPDATE RUN TOOK: " << start.msecsTo(QDateTime::currentDateTime()) << "ms";
    return start.msecsTo(QDateTime::currentDateTime());

}

void ControlModel::resetEmitter(int index, bool ifAdd, int force)
{
    // set type already done from qml!!
    cell *emitter = cells.at(index);
    emitter->force = force;
    if (ifAdd)
    {
        if (!emitters.contains(emitter))
            emitters << emitter;
    }
    else
        emitters.removeOne(emitter);
    // request sim update
}

void ControlModel::CheckNeibors(float *x, float *y, float vx, float vy, float force, float dst, int gen) {
    float x1 = *x, x2 = *x+vx, y1 = *y, y2 = *y+vy;
    int xb1 = round(x1), xb2 = round(x2), yb1 = round(y1), yb2 = round(y2);
    if(!cells_system.atSystem(xb2,yb2))
    {
        return;
    }

    cell *c = cells[cells_system.toLinear(xb1,yb1)];
    QVector <cell*> neibors = c->rneibours();

    if(xb1!=xb2 || yb2!=yb1) {
        float k = (y1-y2)/(x2-x1);
        float b = y1 - k*x1;
        float xang = (float)((xb2+xb1)/2);
        float yang = (float)((yb2+yb1)/2);
        float y3 = k*xang+b;
        float x3 = (yang - b)/k;//формула исходного луча
        //то raycast делать от координат x3 = (yang - b)/k и yang
        //силу луча брать как force*отражение от блока, куда идём
        //луч идёт вверх-вправо

        if(vx>0 && vy<0) {
            if(y3>yang && c->wallstate & 0x40) { //то есть если надо идти в точку вверх
                raycast(x3,yang,vx,-vy,force*neibors[6]->reflect,dst+STEP ,gen+1,true);
            }
            else if(y3<yang && c->wallstate & 0x2) { //то есть идём вправо
                raycast(xang,y3,-vx,vy,force*neibors[0]->reflect,dst+STEP ,gen+1,true);
                //если это стена, raycast по углу 180 минус наш угол
            }
        }
        else if(vx>0 && vy>0) {
            if(y3>yang && c->wallstate & 0x2) { //то есть если надо идти в точку вправо
                raycast(xang,y3,-vx,vy,force*neibors[0]->reflect,dst+STEP ,gen+1,true);
                //если это стена, raycast по углу 540 минус наш угол
            }
            else if(y3<yang && c->wallstate & 0x8) { //то есть если надо идти в точку вниз
                raycast(x3,yang,vx,-vy,force*neibors[2]->reflect,dst+STEP ,gen+1,true);
                //если это стена, raycast по углу 360 минус наш угол
            }
       }
       else if(vx<0 && vy>0) {
            if(y3>yang && c->wallstate & 0x20) { //то есть если надо идти в точку влево
                raycast(xang,y3,-vx,vy,force*neibors[4]->reflect,dst+STEP ,gen+1,true);
                //если это стена, raycast по углу 360 минус наш угол
            }
            else if(y3<yang && c->wallstate & 0x8) { //то есть если надо идти в точку вниз
                raycast(x3,yang,vx,-vy,force*neibors[2]->reflect,dst+STEP ,gen+1,true);
                //если это стена, raycast по углу 540 минус наш угол
            }
       }
       else if(vx<0 && vy<0){
            if(y3>yang && c->wallstate & 0x40) { //то есть если надо идти в точку вверх
                raycast(x3,yang,vx,-vy,force*neibors[6]->reflect,dst+STEP ,gen+1,true);
                //если это стена, raycast по углу 360 минус наш угол
            }
            else if(y3<yang && c->wallstate & 0x20) { //то есть если надо идти в точку влево
                raycast(xang,y3,-vx,vy,force*neibors[4]->reflect,dst+STEP ,gen+1,true);
                //если это стена, raycast по углу 180 минус наш угол
            }

        }
    }
    else
    { //если у нас нет казуса с углами и луч идёт прямо в стену

        //если следующая клетка (xb2,yb2) - стена
        if(yb2<yb1 && c->wallstate & 0x40) { //если луч вверх идёт
            raycast(x2,(float)((yb2+yb1)/2),vx,-vy,force*neibors[6]->reflect,dst+STEP ,gen+1,true);
            //raycast из точки x2,y2 (пойдёт) с углом 360 минус наш угол
        }
        else if(yb2>yb1 && c->wallstate & 0x8) { //если луч вниз идёт
            raycast(x2,(float)((yb2+yb1)/2),vx,-vy,force*neibors[2]->reflect,dst+STEP ,gen+1,true);
            //raycast из точки x2,y2 (пойдёт) с углом 360 минус наш угол
        }
        else if(xb2<xb1 && c->wallstate & 0x20) { //если луч влево идёт
            raycast((float)((xb2+xb1)/2),y2,-vx,vy,force*neibors[4]->reflect,dst+STEP ,gen+1,true);
            //raycast из точки x2,y2 (пойдёт) с углом 540 минус наш угол
        }
        else if(xb2>xb1) { //если луч вправо идёт
            raycast((float)((xb2+xb1)/2),y2,-vx,vy,force*neibors[0]->reflect,dst+STEP ,gen+1,true);
            //raycast из точки x2,y2 (пойдёт) с углом 540 минус наш угол
        }

    //если стена - луч, который идёт по прежнему пути, силушку его домножить на коэф пропуска
    //если не стена - продолжить луч по-мусульмански
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

void ControlModel::raycastX(cell *c)
{
    /// GEN 2 LO bytes is actual gen, HI bytes -- amount of ticks

    // add force to first cell
    float noise = c->getNoise();
    noise = 10 * log10f(powf(10, 0.1f * noise) + powf(10, 0.1f * c->force));
    c->setNoise(noise);
    // set it as visited
    c->visited << 0;

    // - loop for 360 dgs
    // - do:
    float step = 20. / (float)(c->force * c->force);
    //float step = 3.14f / 4.f;
    for (float a = 0; a <= 6.29; a += step) // post point to queue with x y vx vy
        queue.enqueue({(float)c->force, 0, (float)c->x, (float)c->y, STEP * sinf(a), STEP * cosf(a)});

    npacket packet; int c_index; bool atWall;
    while (!queue.isEmpty())
    {
        packet = queue.dequeue();
        if ((packet.gen & 0xFFFF) > MAX_GEN_CNT)
            continue;
        // get cur cell
        c_index = cells_system.toLinear(round(packet.x), round(packet.y));
        if (c_index == -1)
            continue;
        cell *c = cells[c_index];

        float dst = (packet.gen >> 16) * STEP;
        // if we hit a wall
        atWall = c->wallstate & 1;
        if (atWall) //TEMP!!!!
        {
            // check reflection
            CheckNeibors(&packet.x, &packet.y, packet.vx, packet.vy, packet.force, dst, packet.gen);
            // absorbtion
            packet.force *= (100.f - c->absorb) / 100.f;
        }
        // add force to cell
        if (!c->visited.contains((packet.gen & 0xFFFF)) && (packet.gen & 0xFFFF) >= 0)
        {
            float noise = c->getNoise();
            float real_force = packet.force - 6*log2f(dst) - 11.f;
            noise = 10 * log10f(powf(10, 0.1f * noise) + powf(10, 0.1f * real_force));//sqrt((c->noise * c->noise) + (force*force));//+= force;
            c->setNoise(noise);
            c->visited << (packet.gen & 0xFFFF);
        }
        // mov point

        // post next point to queue (with own x, y, vx, vy, force, gen)
        packet.x += packet.vx;
        packet.y += packet.vy;
        queue.enqueue({packet.force, packet.gen + (1 << 16),
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
        queue.enqueue({(float)force, 0, (float)c->x, (float)c->y, STEP * sinf(a), STEP * cosf(a)});

    npacket packet; cell *next, *cur, *reflto;
    while (!queue.isEmpty())
    {
        packet = queue.dequeue();
        if (packet.gen > MAX_GEN_CNT)
            continue;

        //cur = packet.c;
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
        queue.enqueue({/*next,*/ packet.force, packet.gen,
                       packet.x, packet.y, packet.vx, packet.vy});
    }
}


void ControlModel::raycast(float x, float y, float vx, float vy, float force, float dst, int gen, bool atWall)
{
    int c_index = cells_system.toLinear(round(x), round(y));
    if (c_index == -1)
        return;
    if ((gen & 0xFFFF) > MAX_GEN_CNT)
        return;
    float real_force = force;
    if (dst > 0.1f)
        real_force = force - 6*log2f(dst) - 11.f;

    while (real_force > 1.f)
    {
        // get cur cell
        c_index = cells_system.toLinear(round(x), round(y));
        if (c_index == -1)
            break;
        cell *c = cells[c_index];
        // if we hit a wall
        if (c->wallstate != 0) //TEMP!!!!
        {
            CheckNeibors(&x, &y, vx, vy, force, dst, gen);
        }
        atWall = c->absorb != -1;
        if (atWall) // TEMP!!!!
        {
            //qInfo() << "hit a wall ";
            force *= (100.f - c->absorb) / 100.f;
        }
        // add force to cell
        if ((!c->visited.contains(gen) || gen != 0) && gen >= 0)
        {
            float noise = c->getNoise();
            noise = 10 * log10f(powf(10, 0.1f * noise) + powf(10, 0.1f * real_force));//sqrt((c->noise * c->noise) + (force*force));//+= force;
            c->setNoise(noise); // TEMP!!!!
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


bool ControlModel::loadTask(QString taskname)
{
    tasks.clear();
    QFile *taskfile;
    QDataStream *datastream = storage.openDataStream(taskname, &taskfile);
    if (datastream == nullptr)
        return false;

    qint16 header;
    *datastream >> header;
    if (header != FILE_HEADER)
        return false;

    qint32 nTasks;
    *datastream >> nTasks; // read amount of tasks

    qint8 id; qint32 tgt, arg;
    for (int i = 0; i < nTasks; i++)
    {
        *datastream >> id >> tgt >> arg;
        tasks.append(new task(id, tgt, arg));
    }
    return ldFromFile(taskname + "_lvl");
}

bool ControlModel::parseTasks(QString taskname)
{
    tasks.clear();
    // read tsks data
    QVector<QString> data = storage.loadFromFile(taskname + ".raw");
    if (data.isEmpty())
        return false;

    // convert it to tsks
    for (QString dstr : data)
    {
        if (dstr.startsWith('#'))
            continue;
        QStringList splitted = dstr.split(',');
        if (splitted.length() != 4)
            continue;
        qint8 id = ((qint8)(splitted[0].toInt()) << 4) | (qint8)splitted[1].toInt();
        qint32 tgt = (qint32)splitted[2].toInt(), arg = (qint32)splitted[3].toInt();;
        tasks.append(new task(id, tgt, arg));
    }

    // write a file
    QFile *taskfile;
    QDataStream *datastream = storage.openDataStream(taskname, &taskfile);
    if (datastream == nullptr)
        return false;

    *datastream << (qint16)FILE_HEADER;
    *datastream << (qint32)tasks.length();
    for (task *t : tasks)
    {
        *datastream << t->id << t->tgt << t->argument;
    }

    taskfile->flush();
    taskfile->close();
    return true;
}

QQmlListProperty<task> ControlModel::getTasks()
{
    return QQmlListProperty<task>(this, &tasks);
}

void ControlModel::checkCurTasks()
{
    for (task *t : tasks)
        t->reset();
    for (cell* c : cells)
        for (task *t : tasks)
            t->check(c);
}

bool ControlModel::saveToFile(QString filename)
{
    // get cur fld sz
    int width, height;
    cells_system.get(width, height);

    QFile *curfile;
    QDataStream *datastream = storage.openDataStream(filename, (&curfile));
    bool result = datastream != nullptr;
    if (!result)
        return result;

    *datastream << FILE_HEADER_LVL;
    *datastream << width << height;
    for (const cell *c : qAsConst(cells))
        *datastream << CELL_TYPES[c->getType()] << (qint8)c->locked << (qint32)c->force;

    curfile->flush();
    curfile->close();
    return result;


//    QVector<QString> data = QVector<QString>();
//    // save cur sz
//    data.append(QString::number(width));
//    data.append(QString::number(height));
//    // save points
//    for (const cell *c : qAsConst(cells))
//    {
//        data.append(c->getType() /*+ ',' + QString(c->locked) + ','*/);
//    }
//    return storage.saveToFile(filename, data);
}
bool ControlModel::ldFromFile(QString filename)
{
    QFile *curfile;
    QDataStream *datastream = storage.openDataStream(filename, &curfile);
    bool result = datastream != nullptr;
    if (!result)
        return result;

    qint16 header;
    *datastream >> header;
    if (header != FILE_HEADER_LVL)
        return false;

    // get cur fld sz
    int width, height;
    *datastream >> width >> height;

    // ld cell data
    cells_system.set(width, height);
    emit ControlModel::mapReady(); // temp here to update view

    qInfo("beginResetModel");
    beginResetModel();
    // clear old cells
    cells.clear();
    emitters.clear();
    // add new
    int i, x, y; qint8 key; qint8 locked; qint32 force;
    for (i = 0; i < cells_system.maxLinear(); i++)
    {
        *datastream >> key >> locked >> force;

        cells_system.toBilinear(i, x, y);
        cells << new cell(x, y, i, CELL_TYPES.key(key), locked, this);
        cells.last()->force = force;
        if (cells.last()->getType() == "Emitter")
            emitters << cells.last();
    }


    reset_neibours();
    // update cells wallstate
    for (cell *cur : cells)
        if (cur->wallstate & 1)
        {
            i = 0;
            for (cell *c : cur->rneibours())
            {
                if (c != nullptr)
                    c->checkWallstate((i + 4) % 8); // do it not for all -- only for changed
                i++;
            }
        }

    qInfo("endResetModel");
    endResetModel();

    curfile->close();
    return result;
    // get data
//    QVector<QString> data = storage.loadFromFile(filename);
//    if (data.isEmpty())
//        return false;

//    // get fld sz
//    int width = data.takeFirst().toInt();
//    int height = data.takeFirst().toInt();
//    gen(width, height, data);

    //    return true;
}

bool ControlModel::requestAdminKey()
{
    QFile *taskfile;
    QDataStream *datastream = storage.openDataStream("license.lic", &taskfile);
    if (datastream == nullptr)
        return false;

    qint16 header, control = 0xFFFF;
    while (!datastream->atEnd())
    {
        *datastream >> header;
        control ^= header;
    }
    return !(control ^ 0x7B33);
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
        case locked_role: return QVariant(cur->locked);
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
        case locked_role: cur->locked = value.toBool(); break;
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
    names[locked_role] = "locked";
    return names;
}



