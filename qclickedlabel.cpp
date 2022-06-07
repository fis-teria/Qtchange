#include "qclickedlabel.h"
#include "stackedpoint.h"
#include <QtCore/QDebug>

QClickedLabel::QClickedLabel(QWidget *parent)
    :QLabel(parent)
{
}

void QClickedLabel::mousePressEvent(QMouseEvent *e)
{
    emit clickedLabel();
    StackedPoint::pushPoint(e->pos());
    qDebug() << e->pos();

}

