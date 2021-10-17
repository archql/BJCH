#ifndef CELL_H
#define CELL_H

#include <QObject>
#include <QVector>

class cell : public QObject
{
    Q_OBJECT
public:
    explicit cell(QObject *parent = nullptr);
    cell(int start_x, int start_y, int end_x, int end_y, int size, QObject *parent = nullptr);
    ~cell();

    int start_x;
    int end_x;
    int start_y;
    int end_y;
    int size;

    Q_PROPERTY(int start_x MEMBER start_x)
    Q_PROPERTY(int end_x MEMBER end_x)
    Q_PROPERTY(int start_y MEMBER start_y)
    Q_PROPERTY(int end_y MEMBER end_y)
    Q_PROPERTY(int size MEMBER size)



signals:

};

#endif // CELL_H
