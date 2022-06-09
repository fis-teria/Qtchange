#ifndef CAMERAMOVIE_H
#define CAMERAMOVIE_H
#include <QObject>
#include <opencv2/opencv.hpp>
#include <QtMultimedia>

class CameraMovie : public QThread
{
    Q_OBJECT
public:
    explicit CameraMovie(QObject *parent =0);
    QImage cameraCapture();
};

#endif // CAMERAMOVIE_H
