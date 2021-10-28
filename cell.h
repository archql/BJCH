#ifndef CELL_H
#define CELL_H

#include <QObject>
#include <QVector>
#include <QColor>
#include <string.h>
class cell : public QObject
{
    Q_OBJECT
public:
    explicit cell(QObject *parent = nullptr);
    cell(int x, int y, QObject *parent = nullptr);
    ~cell();

    QVector<cell *> &rneibours();
    void setNeibours(cell *p0, cell *p1, cell *p2, cell *p3); 
    int x;
    int y;
    double noise;
    QString snoise;
    QColor color;
    QString typeOfCell;
    int absorb;
    int reflect;
    Q_PROPERTY(int x MEMBER x)
    Q_PROPERTY(int y MEMBER y)
    Q_PROPERTY(double noise MEMBER noise)
    Q_PROPERTY(QColor color MEMBER color)
    Q_PROPERTY(QString typeOfCell MEMBER typeOfCell)
    Q_PROPERTY(int absorb MEMBER absorb)
    Q_PROPERTY(int reflect MEMBER reflect)
    QColor getNoiseColor();

signals:

private:
    QVector<cell *> Neibours;

};

#endif // CELL_H
