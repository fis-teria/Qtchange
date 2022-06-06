#include "qclickedlabel.h"

void QClickedLabel::mousePressEvent(QMouseEvent *e)
{
    emit clickedLabel();
}
