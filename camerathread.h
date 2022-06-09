#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H

#include <QThread>
#include <QTimer>

class cameraThread : public QThread
{
    Q_OBJECT
public:
    explicit cameraThread(QObject *parent = nullptr);
    void run();

signals:
    void data_update(int i);

public slots:
    void timerHit();

};

#endif // CAMERATHREAD_H
