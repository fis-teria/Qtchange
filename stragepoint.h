#ifndef STRAGEPOINT_H
#define STRAGEPOINT_H
#include <QPoint>

class stragePoint
{
private:
    static QPoint p;

public:
    static void getPoint(QPoint pos);
    static QPoint popPoint();

};

#endif // STRAGEPOINT_H
