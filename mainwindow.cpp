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
cv::Mat MainWindow::view = cv::Mat(1,1,CV_8UC3);

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
    /*
    QMediaCaptureSession captureSession;
    camera = new QCamera;
    captureSession.setCamera(camera);
    captureSession.setVideoOutput(ui->viewFind);
    ui->viewFind->show();
    camera->start();*/

    ui->notice->setText(QString::fromLocal8Bit("画像の抽出したい部分を選んでください"));
    ui->noticeCheck->setText(QString::fromLocal8Bit("この部分を抽出するのでよろしいですか？"));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::periodic_works(int i)
{
    cv::VideoCapture cap(0);
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
    int pageNum = ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentIndex(pageNum + 1);

    if(pageNum + 1 == 1){
        cameraOn();
    }
    else
        cameraOff();
}

void MainWindow::buttonBack()
{
    int pageNum = ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentIndex(pageNum - 1);
}

void MainWindow::buttonRetake()
{
    ui->stackedWidget->setCurrentIndex(1);
    cameraOn();
}

void MainWindow::takePicture()
{
    //cameraOff();
    cv::Mat dst;
    cv::VideoCapture cap(0);
    cap >> dst;
    cv::resize(dst,dst,cv::Size(),0.6,0.6);
    imageAccess::strageImage(dst);
    //dst = colorChange(dst);
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
    qDebug("d");
    switch(cameraflg){
    case 1:
        qDebug("c");
        cameraflg =0;
        camera_thread.quit();
        break;
    default:
        break;
    }
}

void MainWindow::cameraOn()
{
    qDebug("a");
    switch (cameraflg) {
    case 0:
        qDebug("b");
        cameraflg=1;
        camera_thread.start();
        qDebug("after thread start");
        break;
        default:
        break;
    }
    qDebug("camera ON end");
}

bool MainWindow::cameraCheck()
{
    return cameraflg;
}


void MainWindow::positionOfImage()
{
    qDebug() << "positionOfImage() start";
    QPoint p = StackedPoint::getPoint();
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
    qDebug() << "a";
    if(((p.x()) < s.x()) && (p.y() < s.y())){
        qDebug() << "b";
        colortake(img,bGR,p.x(),p.y());
        for(int i=0 ;i < trimmed.cols; i++){
            for(int j= 0; j < trimmed.rows; j++){
                trimmed.at<cv::Vec3b>(j,i)[0] = bGR[0];
                trimmed.at<cv::Vec3b>(j,i)[1] = bGR[1];
                trimmed.at<cv::Vec3b>(j,i)[2] = bGR[2];
            }
        }
        qDebug("BGR = %d, %d, %d ", bGR[0] , bGR[1] , bGR[2]);
        cv::cvtColor(trimmed,trimmed,cv::COLOR_BGR2RGB);
        qDebug()<< "e";
        QImage checkImg(trimmed.data, trimmed.cols, trimmed.rows, QImage::Format_RGB888);
        ui->captureSelect->setPixmap(QPixmap::fromImage(checkImg));
        buttonNext();
    }
}






