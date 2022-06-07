#ifndef STACKEDPOINT_H
#define STACKEDPOINT_H

#include <QPoint>

class StackedPoint
{
private:
    static QPoint p;
public:
    static void pushPoint(QPoint pos);
    static QPoint getPoint();
};

#endif // STACKEDPOINT_H
