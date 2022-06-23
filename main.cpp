#include "mainwindow.h"
//#include "../enshu3/colordetective.h"
#include <stdlib.h>
#include <QApplication>
#include <QtCore/QDebug>

int main(int argc, char *argv[])
{
    qDebug("start");
    QApplication a(argc, argv);
    qDebug("mainsindow");
    MainWindow w;
    //Gui w;
    qDebug("show");
    w.show();
    return a.exec();
}
