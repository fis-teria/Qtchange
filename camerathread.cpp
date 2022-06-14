#include "camerathread.h"
#include <stdio.h>
#include <QtCore/QDebug>

cameraThread::cameraThread(QObject *parent)
    : QThread{parent}
{

}
void cameraThread::run()
{
    QTimer timer;
    connect(&timer,&QTimer::timeout, this, &cameraThread::timerHit);

    timer.setInterval(200);
    timer.start();
    qDebug("debug:Thread Start\n");
    exec();
    qDebug("debug: Thread Stops\n");

    timer.stop();
}

void cameraThread::timerHit()
{
    //qDebug("timer hit\n");fflush(stdout);
    static int i;
    emit data_update(i++);
}
