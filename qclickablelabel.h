#ifndef QCLICKABLELABEL_H
#define QCLICKABLELABEL_H

#include <QLabel>

class QClickableLabel : public QLabel
{
    Q_OBJECT
public:
    QClickableLabel();
    ~QClickableLabel();
signals:
    clickedLabel();

protected:
    void mousePressEvent(QMouseEvent *e);

};

#endif // QCLICKABLELABEL_H
