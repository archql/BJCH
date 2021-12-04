#ifndef COORDSSYSTEM_H
#define COORDSSYSTEM_H

#include <QPoint>
#include <QDebug>
//#include <vector.h>


class CordSystem
{
public:
    CordSystem();
    CordSystem(const int max_x,const int max_y);
    CordSystem(const int min_x,const int min_y,const int max_x,const int max_y);

    void set(const int max_x,const int max_y); //max is no relative! (max != width of coord system!)
    void set(const int min_x,const int min_y,const int max_x,const int max_y);
    int maxLinear();//ret total number of points
    void get(int &max_x,int &max_y);
    void get(int &min_x,int &min_y,int &max_x,int &max_y);

    int getWidth();
    int getHeight();

    void toSystem(double &x, double &y);//ret coords In SYSTEM! (nonrelative to min)
    void toSystem(int &x, int &y);//ret coords In SYSTEM! (nonrelative to min)
    void toSystem(QPointF &p);//ret coords In SYSTEM! (nonrelative to min)

    bool atSystem(const int x, const int y);
    bool atSystem(const float x, const float y);

    int toLinear(const int x,const int y);//input nonrelative
    void toBilinear(const int i,int &x,int &y);//i is relative (from 0), other -- no

    //QPointF translateTo(CoordsSystem sys_2,QPointF translate_point);
    QPointF translateTo(QPoint sys_2,QPointF translate_point);
    QPointF translateTo(CordSystem sys_2,QPointF translate_point);
    QPointF translateFrom(QPoint sys_2,QPointF translate_point);
    QPointF translateFrom(CordSystem sys_2,QPointF translate_point);

    QPointF scaleFrom(CordSystem sys_2,QPointF translate_point);
    QPointF scaleTo(CordSystem sys_2,QPointF translate_point);

    //double interpolation(double x,double y,const QList<double> &data);
    //void migration(int x,int y,Vector wind,const QList<double> &data, QList<double> &to);


private:
    int max_x;
    int max_y;
    int min_x;
    int min_y;

    void toSystem();
};

#endif // COORDSSYSTEM_H
