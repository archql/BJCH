#ifndef CELL_H
#define CELL_H

#include <QObject>
#include <QVector>
#include <QSet>
#include <QColor>
#include <QDebug>
#include <string.h>


class cell : public QObject
{
    Q_OBJECT
public:
    explicit cell(QObject *parent = nullptr);
    cell(int x, int y, int index, QObject *parent = nullptr);
    cell(int x, int y, int index, QString cellType, bool locked, QObject *parent = nullptr);
    ~cell();

    void setNoise(const float noise);
    void forceSetNoise(const float noise); // used to reset value wout any updates
    void setType(const QString cellType);

    void checkWallstate(const int index);

    float getNoise() const;
    QString getType() const;

    QVector<cell *> &rneibours();
    //void setNeibours(cell *p0, cell *p1, cell *p2, cell *p3);
    void setNeibours(cell *p0, cell *p1, cell *p2, cell *p3, cell *p4,
                     cell *p5, cell *p6, cell *p7);
    int x, y, index;
    //QString snoise;
    QSet<int> visited;
    QColor color;
    int absorb;
    int reflect;
    int wallstate;
    bool locked;

    int force;

    QString texture;
	
    Q_PROPERTY(int x MEMBER x)
    Q_PROPERTY(int y MEMBER y)
    Q_PROPERTY(int index MEMBER index)
    Q_PROPERTY(float noise MEMBER noise)
    Q_PROPERTY(QColor color MEMBER color)
    Q_PROPERTY(QString typeOfCell MEMBER typeOfCell)
    Q_PROPERTY(int absorb MEMBER absorb)
    Q_PROPERTY(int reflect MEMBER reflect)
    Q_PROPERTY(int wstate MEMBER wallstate)
    Q_PROPERTY(int locked MEMBER locked)
    QColor getNoiseColor();

private:
    QVector<cell *> Neibours;
    float noise;
    QString typeOfCell;
};

#endif // CELL_H
