#pragma execution_character_set("utf-8")
#include "mainwindow.h"
#include "imageaccess.h"
#include "qclickedlabel.h"
#include "stackedpoint.h"
#include "camerathread.h"

#include "./ui_mainwindow.h"
#include <QMouseEvent>
#include <QtMultimedia>
#include <QtMultimediaWidgets>
#include <QMediaCaptureSession>
#include <QVideoWidget>
#include <QtCore/QDebug>
#include <string>
#include <iostream>

int MainWindow::cameraflg = 0;
int MainWindow::pageNum = 0;
cv::Mat MainWindow::view = cv::Mat(1,1,CV_8UC3);
cv::Mat MainWindow::dst = cv::Mat(1,1,CV_8UC3);
cv::VideoCapture MainWindow::cap(0);
cv::Mat MainWindow::theme = cv::Mat(1,1,CV_8UC3);
cv::Mat MainWindow::player_col = cv::Mat(1,1,CV_8UC3);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(&camera_thread,&cameraThread::data_update, this, &MainWindow::periodic_works);
    bool cameraCheck;
    cameraCheck = checkCameraAvailability();
    if(cameraCheck == false)
        qDebug() << "not use Camera";

    ui->notice->setText(QString::fromLocal8Bit("画像の抽出したい部分を選んでください"));
    ui->noticeCheck->setText(QString::fromLocal8Bit("この部分を抽出するのでよろしいですか？"));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::periodic_works(int i)
{
    cap >> view;
    cv::resize(view,view,cv::Size(),0.6,0.6);
    cv::cvtColor(view,view,cv::COLOR_BGR2RGB);
    QImage img(view.data, view.cols, view.rows, QImage::Format_RGB888);
    ui->cameraView->setPixmap(QPixmap::fromImage(img));

}



void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint p = event -> pos();
    StackedPoint::pushPoint(p);
}



void MainWindow::buttonNext()
{
    pageNum = ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentIndex(pageNum + 1);

    if(pageNum + 1 == 1){
        cameraOn();
    }
    else
        cameraOff();
}

void MainWindow::buttonBack()
{
    pageNum = ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentIndex(pageNum - 1);
}

void MainWindow::buttonRetake()
{
    ui->stackedWidget->setCurrentIndex(1);
    cameraOn();
}

void MainWindow::takePicture()
{
    cap >> dst;
    cv::resize(dst,dst,cv::Size(),0.6,0.6);
    imageAccess::strageImage(dst);
    cv::cvtColor(dst,dst,cv::COLOR_BGR2RGB);
    QImage img(dst.data, dst.cols, dst.rows, QImage::Format_RGB888);
    buttonNext();
    ui->captureImage->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::colortake(const cv::Mat &src, int num[3], int xtouch, int ytouch)
{
    num[0] = 0, num[1] = 0, num[2] = 0;
    if(xtouch - 10 < 0)  xtouch += 10;
    if(xtouch + 10 > src.cols)  xtouch -= 10;
    if(ytouch - 10 < 0)  ytouch += 10;
    if(ytouch + 10 > src.rows)  ytouch -= 10;

    for(int x = -10; x < 10; x++){
        for(int y = -10; y < 10; y++){
            num[0] += src.at<cv::Vec3b>(y+ytouch,x+xtouch)[0];
            num[1] += src.at<cv::Vec3b>(y+ytouch,x+xtouch)[1];
            num[2] += src.at<cv::Vec3b>(y+ytouch,x+xtouch)[2];
        }
    }
    num[0] = num[0]/441;
    num[1] = num[1]/441;
    num[2] = num[2]/441;
}

bool MainWindow::checkCameraAvailability()
{
    if(QMediaDevices::videoInputs().count()>0)
        return true;
    return false;
}

void MainWindow::cameraOff()
{
    switch(cameraflg){
    case 1:
        cameraflg =0;
        camera_thread.quit();
        break;
    default:
        break;
    }
}

void MainWindow::cameraOn()
{
    switch (cameraflg) {
    case 0:
        cameraflg=1;
        camera_thread.start();
        break;
    default:
        break;
    }
}

void MainWindow::positionOfImage()
{
    qDebug() << "positionOfImage() start";
    QPoint p = StackedPoint::getPoint();
    qDebug() << p;
    QPoint s;
    int bGR[3] = {0};
    cv::Mat img;
    img = imageAccess::returnImage(img);
    cv::Mat trimmed(img.rows,img.cols,CV_8UC3);
    s.setX(img.cols);
    s.setY(img.rows);
    std::string x = std::__cxx11::to_string(p.x());
    std::string y = std::__cxx11::to_string(p.y());
    std::string sx = std::__cxx11::to_string(s.x());
    std::string sy = std::__cxx11::to_string(s.y());
    if(((p.x()) < s.x()) && (p.y() < s.y())){
        colortake(img,bGR,p.x(),p.y());
        player_col.at<cv::Vec3b>(0,0)[0] = bGR[0];
        player_col.at<cv::Vec3b>(0,0)[1] = bGR[1];
        player_col.at<cv::Vec3b>(0,0)[2] = bGR[2];

        for(int i=0 ;i < trimmed.cols; i++){
            for(int j= 0; j < trimmed.rows; j++){
                trimmed.at<cv::Vec3b>(j,i)[0] = bGR[0];
                trimmed.at<cv::Vec3b>(j,i)[1] = bGR[1];
                trimmed.at<cv::Vec3b>(j,i)[2] = bGR[2];
            }
        }
        qDebug("BGR = %d, %d, %d ", bGR[0] , bGR[1] , bGR[2]);
        cv::cvtColor(trimmed,trimmed,cv::COLOR_BGR2RGB);
        QImage checkImg(trimmed.data, trimmed.cols, trimmed.rows, QImage::Format_RGB888);
        ui->captureSelect->setPixmap(QPixmap::fromImage(checkImg));
        buttonNext();
    }
}

double MainWindow::score_calc(const cv::Mat &ques_rgb, const cv::Mat &player_rgb)
{
    double out_H, out_S, out_V;
    double ques[3], player[3];
    double point;

    cv::Mat ques_hsv(ques_rgb.size(), ques_rgb.type(), cv::Scalar(0, 0, 0));
    cv::Mat player_hsv(player_rgb.size(), player_rgb.type(), cv::Scalar(0, 0, 0));

    cv::cvtColor(ques_rgb, ques_hsv, cv::COLOR_BGR2HSV);
    cv::cvtColor(player_rgb, player_hsv, cv::COLOR_BGR2HSV);

    int y = 0, x = 0;
    for (int i = 0; i < 3; i++)
    {
        ques[i] = ques_hsv.at<cv::Vec3b>(y, x)[i];
        player[i] = player_hsv.at<cv::Vec3b>(y, x)[i];
    }
    //qDebug("quesH = %lf, playerH = %lf",ques[0]*2,player[0]*2);
    //qDebug("quesS = %lf, playerS = %lf",ques[1],player[1]);
    //qDebug("quesV = %lf, playerV = %lf",ques[2],player[2]);

    out_H = (double)(abs(ques[0] - player[0]) / 180) * (1.8) * 100;
    out_S = (double)(abs(ques[1] - player[1]) / 255) * (0.333) * 100;
    out_V = (double)(abs(ques[2] - player[2]) / 255) * (0.333  ) * 100;

    /*
    out_H = (double)((120.0 -100.0) / 180) * (1.8) * 100;
    out_S = (double)((0.0) / 255) * (0.333) * 100;
    out_V = (double)((0.0) / 255) * (0.333  ) * 100;
    */
qDebug("H %lf S %lf V %lf",out_H, out_S, out_V);
    point = 100 - (out_H + out_S + out_V);
    if(point > 100)
        point = 100;
    return point;
}

void MainWindow::ok2Result()
{
    buttonNext();
    for(int i = 0; i<theme.cols; i++){
        for(int j = 0; j < theme.rows; j++){
            theme.at<cv::Vec3b>(j,i)[0] = 187;
            theme.at<cv::Vec3b>(j,i)[1] = 123;
            theme.at<cv::Vec3b>(j,i)[2] = 0;
        }
    }
    double score;
    score = score_calc(theme,player_col);
    std::string str = std::__cxx11::to_string(score);
    ui->getPoint->setText(QString::fromLocal8Bit(str));
    ui->message->setText(QString::fromLocal8Bit("ここにコメントが入るよ！"));
    ui->yours->setText(QString::fromLocal8Bit("あなたが選んだ色はこちらです"));
    ui->quesName->setText(QString::fromLocal8Bit("〇〇色"));
    cv::Mat resizeT(1,1,CV_8UC3);
    cv::Mat resizeP(1,1,CV_8UC3);
    cv::resize(theme,resizeT,cv::Size(),300,300);
    cv::cvtColor(resizeT,resizeT,cv::COLOR_BGR2RGB);
    QImage imgT(resizeT.data, resizeT.cols, resizeT.rows, QImage::Format_RGB888);
    cv::resize(player_col,resizeP,cv::Size(),300,300);
    cv::cvtColor(resizeP,resizeP,cv::COLOR_BGR2RGB);
    QImage imgP(resizeP.data, resizeP.cols, resizeP.rows, QImage::Format_RGB888);
    ui->theme->setPixmap(QPixmap::fromImage(imgT));
    ui->pSelected->setPixmap(QPixmap::fromImage(imgP));
}

void MainWindow::toStart()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::toReplay()
{
    ui->stackedWidget->setCurrentIndex(0);
}






