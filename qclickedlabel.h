#ifndef QCLICKEDLABEL_H
#define QCLICKEDLABEL_H
#include <QMouseEvent>
#include <QLabel>

class QClickedLabel : public QLabel
{
    Q_OBJECT
public:
    QClickedLabel(QWidget *parent);
signals:
    void clickedLabel();

protected:
    void mousePressEvent(QMouseEvent *e);
};
#endif // QCLICKEDLABEL_H
