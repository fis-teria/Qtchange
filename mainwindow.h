#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qclickedlabel.h"
#include "camerathread.h"
#include "../enshu3/start.h"
#include "../enshu3/gamemode.h"
#include "../enshu3/target.h"
#include "../enshu3/targeth.h"
#include "../enshu3/cv.h"
#include "../enshu3/form.h"

#include <stdlib.h>
#include <QMainWindow>
#include <QWidget>
#include <QFileDialog>
#include <opencv2/opencv.hpp>
#include <QCamera>
#include <math.h>
#include <QMouseEvent>
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
    void buttonChangeColor();
    void buttonCredit();
    void takePicture();
    void positionOfImage();
    void ok2Result();
    void toStart();
    void toReplay();
    void easyFlg();
    void hardFlg();
    void next2TargetMode();

private:
    void mousePressEvent(QMouseEvent *event);
    void colortake(const cv::Mat &src, int num[3], int xtouch, int ytouch);
    bool checkCameraAvailability();
    void cameraOff();
    void cameraOn();
    double score_calc(const cv::Mat &ques_rgb, const cv::Mat &player_rgb);
    double score_calc_BGR(const cv::Mat &ques_rgb, const cv::Mat &player_rgb);
    double score_calc_Lch(const cv::Mat &ques_rgb, const cv::Mat &player_rgb);

private:
    Ui::MainWindow *ui;
    cameraThread camera_thread;
    Start *start;
    Gamemode *gamemode;
    Target *easy;
    Targeth *hard;
public:
    static int cameraflg;
    static int modeFlg;
    static int pageNum;
    static cv::Mat view;
    static cv::Mat dst;
    static cv::VideoCapture cap;
    static cv::Mat themeColor;
    static cv::Mat player_col;
};
#endif // MAINWINDOW_H
