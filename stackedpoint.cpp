#include "stackedpoint.h"

QPoint StackedPoint::p(0,0);

void StackedPoint::pushPoint(QPoint pos)
{
    p = pos;
}

QPoint StackedPoint::getPoint()
{
    return p;
}
