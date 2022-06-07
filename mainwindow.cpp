#pragma execution_character_set("utf-8")
#include "mainwindow.h"
#include "imageaccess.h"
#include "qclickedlabel.h"
#include "stackedpoint.h"

#include "./ui_mainwindow.h"
#include <QMouseEvent>
#include <QtMultimedia>
#include <QtMultimediaWidgets>
#include <QtCore/QDebug>
#include <string>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //QMediaCaptureSession captureSession;
    //camera = new QCamera;
    //captureSession.setCamera(camera);

    //captureSession.setVideoOutput(ui->viewFinder);
    //ui->viewFinder->show();
    //camera->start();
    ui->notice->setText(QString::fromLocal8Bit("画像の抽出したい部分を選んでください"));
    ui->noticeCheck->setText(QString::fromLocal8Bit("この部分を抽出するのでよろしいですか？"));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openImage()
{
    ui->label->setPixmap(QPixmap::fromImage(QImage(QFileDialog::getOpenFileName())));

}

cv::Mat MainWindow::colorChange(cv::Mat img)
{
    cv::rotate(img,img,cv::ROTATE_90_CLOCKWISE);
    return img;
}

char* MainWindow::mojiArray()
{
    char *data1 = "現実";
    char *data2 = "さようなら";
    char *data3 = "ありがとう";
    char *data4 = "無理";
    int x;
    x = rand() % 4;

    switch(x){
    case 0:
        return data1;
    case 1:
        return data2;
    case 2:
        return data3;
    default:
        return data4;
        break;
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint p = event -> pos();
    StackedPoint::pushPoint(p);
}


void MainWindow::mojiHyouji()
{
    std::string str(mojiArray());
    ui->lineEdit->setText(QString::fromLocal8Bit(str));
}

void MainWindow::changeHorizon()
{
    cv::Mat dst;
    cv::Mat small;
    cv::VideoCapture cap(0);
    cap >> dst;
    cv::resize(dst,dst,cv::Size(),0.6,0.6);
    imageAccess::strageImage(dst);
    //dst = colorChange(dst);
    cv::cvtColor(dst,dst,cv::COLOR_BGR2RGB);
    QImage img(dst.data, dst.cols, dst.rows, QImage::Format_RGB888);
    ui->camearaView->setPixmap(QPixmap::fromImage(img));

}

void MainWindow::buttonNext()
{
    int pageNum = ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentIndex(pageNum + 1);
}

void MainWindow::buttonBack()
{
    int pageNum = ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentIndex(pageNum - 1);
}

void MainWindow::buttonRetake()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::takePicture()
{
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

void colortake(const cv::Mat &src, int num[3], int xtouch, int ytouch)
{
  num[0] = 0, num[1] = 0, num[2] = 0;
  if(xtouch - 10 < 0)  xtouch += 10;
  if(xtouch + 10 > src.cols)  xtouch -= 10;
  if(ytouch - 10 < 0)  ytouch += 10;
  if(ytouch + 10 > src.rows)  ytouch -= 10;
​
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


void MainWindow::positionOfImage()
{
    QPoint p = StackedPoint::getPoint();
    QPoint s;
    cv::Mat img;
    img = imageAccess::returnImage(img);
    s.setX(img.cols);
    s.setY(img.rows);
    std::string x = std::__cxx11::to_string(p.x());
    std::string y = std::__cxx11::to_string(p.y());
    std::string sx = std::__cxx11::to_string(s.x());
    std::string sy = std::__cxx11::to_string(s.y());

    if(((p.x()>0) && (p.y() > 0)) && ((p.x()) < s.x()) && (p.y() < s.y())){
        std::string str = "mouse x = " + x + "y = " + y + "label x = " + sx + "y = " + sy;
        ui->lineEdit_2->setText(QString::fromLocal8Bit(str));
    }
}




