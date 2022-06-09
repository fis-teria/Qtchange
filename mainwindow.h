#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stdlib.h>
#include <QMainWindow>
#include <QWidget>
#include <QFileDialog>
#include <opencv2/opencv.hpp>
#include <QCamera>
#include <math.h>
#include <QMouseEvent>
#include "qclickedlabel.h"
#include "camerathread.h"
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void periodic_works(int i);

private slots:
    void buttonNext();
    void buttonBack();
    void buttonRetake();
    void takePicture();
    void positionOfImage();

private:
    cv::Mat colorChange(cv::Mat img);
    void mousePressEvent(QMouseEvent *event);
    void colortake(const cv::Mat &src, int num[3], int xtouch, int ytouch);
    bool checkCameraAvailability();
    void useCamera();
    void cameraOff();
    void cameraOn();
    bool cameraCheck();


private:
    Ui::MainWindow *ui;
    cameraThread camera_thread;
public:
    static int cameraflg;
    static cv::Mat view;
};
#endif // MAINWINDOW_H
