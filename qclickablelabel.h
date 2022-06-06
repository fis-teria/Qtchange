#ifndef QCLICKABLELABEL_H
#define QCLICKABLELABEL_H

#include <QLabel>

class QClickableLabel : public QLabel
{
public:
    QClickableLabel();
    ~QClickableLabel();
signs:
    clicked();

protected:
    void mousePressEvent(QMouseEvent *e);

};

#endif // QCLICKABLELABEL_H
