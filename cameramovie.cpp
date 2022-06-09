#include "cameramovie.h"


CameraMovie::CameraMovie(QObject *parent)
    :QObject(parent)
{

}

QImage CameraMovie::cameraCapture()
{
    cv::Mat src;
    cv::VideoCapture cap(0);
    cap >> src;
    cv::resize(src,src,cv::Size(),0.6,0.6);
    //dst = colorChange(dst);
    cv::cvtColor(src,src,cv::COLOR_BGR2RGB);
    QImage mov(src.data, src.cols, src.rows, QImage::Format_RGB888);
    return mov;
}
