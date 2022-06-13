#include "qclickedlabel.h"
#include "stackedpoint.h"
#include <QtCore/QDebug>

QClickedLabel::QClickedLabel(QWidget *parent)
    :QLabel(parent)
{
}

void QClickedLabel::mousePressEvent(QMouseEvent *e)
{
    StackedPoint::pushPoint(e->pos());
    qDebug() << e->pos();
    emit clickedLabel();

}

