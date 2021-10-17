#include "cell.h"

cell::cell(QObject *parent) : QObject(parent)
{

}

cell::cell(int start_x, int start_y, int end_x, int end_y, int size, QObject *parent) : QObject(parent)
{
    this->start_x = start_x;
    this->start_y = start_y;
    this->end_x = end_x;
    this->end_y = end_y;
    this->size = size;
}

cell::~cell()
{

}
