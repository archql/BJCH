#include "cordsystem.h"

CordSystem::CordSystem()
{
}
CordSystem::CordSystem(const int max_x,const int max_y)
{
    set(max_x,max_y);
}
CordSystem::CordSystem(const int min_x,const int min_y,const int max_x,const int max_y)
{
    set(min_x,min_y,max_x,max_y);
}

void CordSystem::set(const int max_x,const int max_y)
{
    this->max_x=max_x;
    this->max_y=max_y;
    min_x=0;
    min_y=0;
}
void CordSystem::set(const int min_x,const int min_y,const int max_x,const int max_y)
{
    this->max_x=max_x;
    this->max_y=max_y;
    this->min_x=min_x;
    this->min_y=min_y;
}
void CordSystem::get(int &max_x,int &max_y)
{
    max_x=this->max_x;
    max_y=this->max_y;
}
void CordSystem::get(int &min_x,int &min_y,int &max_x,int &max_y)
{
    max_x=this->max_x;
    max_y=this->max_y;
    min_x=this->min_x;
    min_y=this->min_y;
}

int CordSystem::maxLinear(){
    return(max_x-min_x)*(max_y-min_y);
}

void CordSystem::toSystem(double &x,double &y)
{
    if(x >= max_x)
        x = 2 * max_x - x - 1; // min_x min y isnt supported!
    else if(x < min_x)
        x = 2* min_x - x;
    if(y >= max_y)
        y = 2 * max_y - y - 1;
    else if(y < min_y)
        y = 2* min_y - y;
}
void CordSystem::toSystem(int &x,int &y)
{
    while (!atSystem(x, y))
    {
        if(x >= max_x)
            x = 2 * max_x - x - 1; // min_x min y isnt supported!
        else if(x < min_x)
            x = 2* min_x - x;
        if(y >= max_y)
            y = 2 * max_y - y - 1;
        else if(y < min_y)
            y = 2* min_y - y;
    }
}
bool CordSystem::atSystem(const int x, const int y)
{
    return (x < max_x && x >= min_x && y < max_y && y >= min_y);
}
bool CordSystem::atSystem(const float x, const float y)
{
    return (x < max_x && x >= min_x && y < max_y && y >= min_y);
}

void CordSystem::toSystem(QPointF &p)
{
    toSystem(p.rx(), p.ry());
}

int CordSystem::toLinear(int x, int y)
{
    if (atSystem(x, y))
        return (y-min_y)*(max_x-min_x)+ x-min_x;
   else
        return -1;
}
void CordSystem::toBilinear(const int i, int &x, int &y)
{
    x = i % (max_x-min_x) + min_x;
    y = i / (max_x-min_x) + min_y;
    toSystem(x, y);
}

QPointF CordSystem::translateTo(QPoint sys_2, QPointF translate_point)
{
    this->toSystem(translate_point.rx(),translate_point.ry());
    double x2=double(sys_2.x())/double(max_x-min_x) * (translate_point.x()-min_x);
    double y2=double(sys_2.y())/double(max_y-min_y) * (translate_point.y()-min_y);
    return QPointF(x2,y2);
}
QPointF CordSystem::translateTo(CordSystem sys_2,QPointF translate_point)
{
    int max_x2,max_y2,min_x2,min_y2;
    sys_2.get(min_x2,min_y2,max_x2,max_y2);
    double x2=double(max_x2-min_x2)/double(max_x-min_x) * (translate_point.x()-min_x)+min_x2;
    double y2=double(max_y2-min_y2)/double(max_y-min_y) * (translate_point.y()-min_y)+min_y2;
    sys_2.toSystem(x2,y2);
    return QPointF(x2,y2);
}

QPointF CordSystem::translateFrom(QPoint sys_2,QPointF translate_point)
{
    double x2=double(max_x-min_x)/double(sys_2.x()) * translate_point.x()+min_x;
    double y2=double(max_y-min_y)/double(sys_2.y()) * translate_point.y()+min_y;
    this->toSystem(x2,y2);
    return QPointF(x2,y2);
}
QPointF CordSystem::translateFrom(CordSystem sys_2,QPointF translate_point)
{
    sys_2.toSystem(translate_point.rx(),translate_point.ry());

    int max_x2,max_y2,min_x2,min_y2;
    sys_2.get(min_x2,min_y2,max_x2,max_y2);
    double x2=double(max_x-min_x)/double(max_x2-min_x2) * (translate_point.x()-min_x2)+min_x;
    double y2=double(max_y-min_y)/double(max_y2-min_y2) * (translate_point.y()-min_y2)+min_y;
    return QPointF(x2,y2);
}
/*
QPointF CoordsSystem::scaleFrom(CoordsSystem sys_2,QPointF translate_point)
{

}
QPointF CoordsSystem::scaleTo(CoordsSystem sys_2,QPointF translate_point)
{

}*/

//double CordSystem::interpolation(double x,double y,const QList<double> &data)
//{

//    //qInfo()<<"cx "<<x<<" cy "<<y;
//    int a=int(x);
//    int b=int(y);
//    //qInfo()<<"a "<<a<<" b "<<b;
//    x-=a;
//    y-=b;
//    //qInfo()<<"cx "<<x<<" cy "<<y;


//    double value_0=data[this->toLinear(a,b)];
//    double value_1=data[this->toLinear(a+1,b)];
//    double value_2=data[this->toLinear(a+1,b+1)];
//    double value_3=data[this->toLinear(a,b+1)];

//    //qInfo()<<"value_0 "<<value_0<<" value_1 "<<value_1<<"value_2 "<<value_2<<" value_3 "<<value_3;
//    //qInfo()<<"value "<<value_0*(1-x)*(1-y)+value_1*x*(1-y)+value_3*(1-x)*y+value_2*x*y;

//    return value_0*(1-x)*(1-y)+value_1*x*(1-y)+value_3*(1-x)*y+value_2*x*y;
//}
//void CordSystem::migration(int x,int y,Vector wind,const QList<double> &data, QList<double> &to)
//{
//    double tile_const=3.6/200*5;//!!!Consts!!!! km/h /km

//    //if(x==1&&y==1)
//    //{
//    /*
//    //if(wind.x()>0){
//        double delta=(data[toLinear(x,y)]-data[toLinear(x+1,y)])*tile_const*abs(wind.x());
//        to[toLinear(x+1,y)]+=delta;
//        to[toLinear(x,y)]-=delta;
//    //}else{
//         delta=(data[toLinear(x,y)]-data[toLinear(x-1,y)])*tile_const*abs(wind.x());
//        to[toLinear(x-1,y)]+=delta;
//        to[toLinear(x,y)]-=delta;
//    //}
//    //if(wind.y()>0){
//         delta=(data[toLinear(x,y)]-data[toLinear(x,y+1)])*tile_const*abs(wind.y());
//        to[toLinear(x,y+1)]+=delta;
//        to[toLinear(x,y)]-=delta;
//    //}else{
//         delta=(data[toLinear(x,y)]-data[toLinear(x,y-1)])*tile_const*abs(wind.y());
//        to[toLinear(x,y-1)]+=delta;
//        to[toLinear(x,y)]-=delta;
//    //}
//        //qInfo()<<"dt down "<<data[toLinear(x,y+1)];
//        //qInfo()<<"wind "<<wind.x()<<" "<<wind.y();
//    //}
//    */

//    double delta=(data[toLinear(x,y)]-data[toLinear(x+1,y)])*tile_const;
//    if(delta < 0){  //!!!!CONSTS!!!
//        //delta=sqrt(abs(delta))*tile_const;
//        to[toLinear(x+1,y)]-=abs(delta);
//        to[toLinear(x,y)]+=abs(delta);
//    }
//    delta=(data[toLinear(x,y)]-data[toLinear(x-1,y)])*tile_const;
//    if(delta < 0){
//        //delta=sqrt(abs(delta))*tile_const;
//        to[toLinear(x-1,y)]-=abs(delta);
//        to[toLinear(x,y)]+=abs(delta);
//    }
//    delta=(data[toLinear(x,y)]-data[toLinear(x,y+1)])*tile_const;
//    if(delta < 0){
//        //delta=sqrt(abs(delta))*tile_const;
//        to[toLinear(x,y+1)]-=abs(delta);
//        to[toLinear(x,y)]+=abs(delta);
//    }
//    delta=(data[toLinear(x,y)]-data[toLinear(x,y-1)])*tile_const;
//    if(delta < 0){
//        //delta=sqrt(abs(delta))*tile_const;
//        to[toLinear(x,y-1)]-=abs(delta);
//        to[toLinear(x,y)]+=abs(delta);
//    }


//}
