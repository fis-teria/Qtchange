#include "stragepoint.h"


void stragePoint::getPoint(QPoint pos)
{
    p = pos;
}

QPoint stragePoint::popPoint()
{
    return p;
}
