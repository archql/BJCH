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
    cell(int x, int y, QObject *parent = nullptr);
    cell(int x, int y, QString cellType, QObject *parent = nullptr);
    ~cell();

    void setNoise(const float noise);
    void setType(const QString cellType);

    void checkWallstate();

    float getNoise() const;
    QString getType() const;

    QVector<cell *> &rneibours();
    //void setNeibours(cell *p0, cell *p1, cell *p2, cell *p3);
    void setNeibours(cell *p0, cell *p1, cell *p2, cell *p3, cell *p4,
                     cell *p5, cell *p6, cell *p7);
    int x;
    int y;
    //QString snoise;
    QSet<int> visited;
    QColor color;
    int absorb;
    int reflect;
    int wallstate;
	
    Q_PROPERTY(int x MEMBER x)
    Q_PROPERTY(int y MEMBER y)
    Q_PROPERTY(float noise MEMBER noise)
    Q_PROPERTY(QColor color MEMBER color)
    Q_PROPERTY(QString typeOfCell MEMBER typeOfCell)
    Q_PROPERTY(int absorb MEMBER absorb)
    Q_PROPERTY(int reflect MEMBER reflect)
    Q_PROPERTY(int wstate MEMBER wallstate)
    QColor getNoiseColor();

signals:
    void selfChanged(const int x, const int y);

private:
    QVector<cell *> Neibours;
    float noise;
    QString typeOfCell;
};

#endif // CELL_H
