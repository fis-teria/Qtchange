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
#include <vector>
#include <random>
#include <ctime>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <unistd.h>


//#define sub
//#define dis
int MainWindow::cameraflg = 0;
int MainWindow::modeFlg = 0;
int MainWindow::pageNum = 0;
cv::Mat MainWindow::view = cv::Mat(1,1,CV_8UC3);
cv::Mat MainWindow::dst = cv::Mat(1,1,CV_8UC3);
cv::VideoCapture MainWindow::cap(0);
cv::Mat MainWindow::themeColor = cv::Mat(1,1,CV_8UC3);
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

    start = new Start;
    gamemode = new Gamemode;
    easy = new Target;
    hard = new Targeth;
    qDebug("a");
    ui->stackedWidget->addWidget(start);
    ui->stackedWidget->addWidget(gamemode);
    ui->stackedWidget->addWidget(easy);
    ui->stackedWidget->addWidget(hard);
    ui->stackedWidget->setCurrentIndex(0);
    int page = ui->stackedWidget->currentIndex();
    int sum = ui->stackedWidget->count();
    qDebug("MainWindow = %d, sum = %d",page, sum);
    qDebug("b");

    //const char dir[256];
    //getcwd(dir,256);

    //std::string logo_str = dir + "/logo.png";
    //std::string det_str = dir + "/tantei.png";
    //std::string logo_str = "/home/user/QtWorkSpace/colordetective-ver.1.0/logo.png";
    //std::string det_str = "/home/user/QtWorkSpace/colordetective-ver.1.0/tantei.png";

    //start
    std::filesystem::path logopath = std::filesystem::current_path();
    logopath.append("logo.png");
    std::filesystem::path tanteipath = std::filesystem::current_path();
    tanteipath.append("tantei.png");
    QPixmap logo(QString::fromStdString(logopath));
    QPixmap det(QString::fromStdString(tanteipath));
    ui -> logoStart -> setPixmap(logo.scaled(240, 129));
    ui -> detStart -> setPixmap(det.scaled(139, 226));
    qDebug("aaaaaaaaaaaaaaaaa");

    //gamemode
    ui -> logoGM -> setPixmap(logo.scaled(240, 129));
    ui -> detGM -> setPixmap(det.scaled(139, 226));
    ui -> messageGM->setText(QString::fromLocal8Bit("難易度選択"));


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

    if(pageNum + 1 == 3){
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
    ui->stackedWidget->setCurrentIndex(3);
    cameraOn();
}

void MainWindow::buttonChangeColor()
{
    std::srand(std::time(nullptr));
    int val = 0, ran = rand();
    cv::Vec3b src;
    std::stringstream str;
    Opencv::random(val, src, ran);
    str = Opencv::jed(val);
    cv::Mat_<cv::Vec3b> mat(200, 400, cv::Vec3b(src[0], src[1], src[2]));
    themeColor.at<cv::Vec3b>(0,0) = mat.at<cv::Vec3b>(0,0);
    if(modeFlg == 1){
        Opencv::setData(val, mat);
        Opencv::waku(mat);
        cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);
        ui -> ImageTarget -> setPixmap(QPixmap::fromImage(QImage((unsigned char*) mat.data, mat.cols, mat.rows, QImage::Format_RGB888)));
        ui -> NameTarget -> setText(QString::fromStdString(str.str()));
        ui -> theme -> setPixmap(QPixmap::fromImage(QImage((unsigned char*) mat.data, mat.cols, mat.rows, QImage::Format_RGB888)));
        ui -> quesName -> setText(QString::fromStdString(str.str()));
    }
    else if(modeFlg == 2){
        Opencv::setData(val, mat);
        ran = rand();
        Opencv::shape(ran, str, mat);
        Opencv::waku(mat);
        cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);
        ui -> ImageTarget -> setPixmap(QPixmap::fromImage(QImage((unsigned char*) mat.data, mat.cols, mat.rows, QImage::Format_RGB888)));
        ui -> NameTarget -> setText(QString::fromStdString(str.str()));
        ui -> theme -> setPixmap(QPixmap::fromImage(QImage((unsigned char*) mat.data, mat.cols, mat.rows, QImage::Format_RGB888)));
        ui -> quesName -> setText(QString::fromStdString(str.str()));
    }
}

void MainWindow::buttonCredit()
{

}

void MainWindow::takePicture()
{
    cap >> dst;
    cv::resize(dst,dst,cv::Size(),0.4,0.4);
    imageAccess::strageImage(dst);
    cv::cvtColor(dst,dst,cv::COLOR_BGR2RGB);
    QImage img(dst.data, dst.cols, dst.rows, QImage::Format_RGB888);
    buttonNext();
    ui->captureImage->setPixmap(QPixmap::fromImage(img));
    ui->retakeGetC->setText(QString::fromLocal8Bit("撮り直し"));

    cv::Mat hint = themeColor.clone();
    cv::resize(hint,hint, cv::Size(),100,100);
    cv::cvtColor(hint,hint,cv::COLOR_BGR2RGB);
    QImage hintImg(hint.data, hint.cols, hint.rows, QImage::Format_RGB888);
    ui->hint->setPixmap(QPixmap::fromImage(hintImg));
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
    if (ques[0] == 0 && abs(0 - player[0]) < abs(180 - player[0]))
    {
        ques[0] = 0;
    }
    else if (ques[0] == 0 && abs(0 - player[0]) >= abs(180 - player[0]))
    {
        ques[0] = 180;
    }
#ifdef sub
    //引き算パターン
    //FILE *fp;
    //fp = fopen("/home/user/Documents/QtResults/result_HSV_AVoD","a");
    out_H = (180 - (double)abs(ques[0] - player[0])) * 100 / 180;
    out_S = (255 - (double)abs(ques[1] - player[1])) * 100 / 255;
    out_V = (255 - (double)abs(ques[2] - player[2])) * 100 / 255;
    // point = (out_H + out_S + out_V)/3;
    //point = out_H * (0.7) + out_S * (0.1) + out_V * (0.2);
    point = out_H * (0.8) + out_S * (0.05) + out_V * (0.15);
    // out_H = (double)(abs(ques[0] - player[0]) / 180) * (1.8) * 100;
    // out_S = (double)(abs(ques[1] - player[1]) / 255) * (1/3) * 100;
    // out_V = (double)(abs(ques[2] - player[2]) / 255) * (0.6) * 100;
    // point = 100 - (out_H + out_S + out_V);
    qDebug("out_H = %f\n", out_H);
    qDebug("out_S = %f\n", out_S);
    qDebug("out_V = %f\n", out_V);
    //fprintf(fp,"theme color = %lf %lf %lf, player colors = %lf %lf %lf\n result = %lf\n",ques[0],ques[1],ques[2],player[0],player[1],player[2],point);
    //fclose(fp);

#elif dis
    // 3次元空間のパターン
    //FILE *fp;
    //fp = fopen("/home/user/Documents/QtResults/result_HSV_Distance","a");
    double dis_H, dis_S, dis_V;
    double distance, max;
    dis_H = (ques[0] - player[0]) * (ques[0] - player[0]);
    dis_S = (ques[1] - player[1]) * (ques[1] - player[1]);
    dis_V = (ques[2] - player[2]) * (ques[2] - player[2]);
    distance = sqrt(dis_H + dis_S + dis_V);
    max = sqrt((180 * 180) + (255 * 255) + (255 * 255));
    point = 100 - (distance * 100 / max);
    qDebug("distance_HSV = %f\n", distance);
    qDebug("max_HSV = %f\n", max);
    qDebug("ques_HSV = %f,%f,%f\n", ques[0], ques[1], ques[2]);
    qDebug("player_HSV = %f,%f,%f\n\n", player[0], player[1], player[2]);
    // cv::waitKey(0);
    //fprintf(fp,"theme color = %lf %lf %lf, player colors = %lf %lf %lf\n result = %lf\n",ques[0],ques[1],ques[2],player[0],player[1],player[2],point);
    //fclose(fp);

#endif
    return point;
}

double MainWindow::score_calc_BGR(const cv::Mat &ques_rgb, const cv::Mat &player_rgb)
{
    double ques[3], player[3];
    double point;
    int y = 0, x = 0;
    for (int i = 0; i < 3; i++)
    {
        ques[i] = ques_rgb.at<cv::Vec3b>(y, x)[i];
        player[i] = player_rgb.at<cv::Vec3b>(y, x)[i];
    }
#ifdef sub
    //FILE *fp;
    //fp = fopen("/home/user/Documents/QtResults/result_BGR_AVoD","a");

    double out_B, out_G, out_R;
    out_B = (255 - (double)abs(ques[0] - player[0])) * 100 / 255;
    out_G = (255 - (double)abs(ques[1] - player[1])) * 100 / 255;
    out_R = (255 - (double)abs(ques[2] - player[2])) * 100 / 255;
    // point = (out_H + out_S + out_V)/3;
    point = (out_B + out_G + out_R) / 3;

    //fprintf(fp,"theme color = %lf %lf %lf, player colors = %lf %lf %lf\n result = %lf \n",ques[0],ques[1],ques[2],player[0],player[1],player[2],point);
    //fclose(fp);
    qDebug("pointBGR = %lf", point);
#elif dis
    //FILE *fp;
    //fp = fopen("/home/user/Documents/QtResults/result_BGR_Distance","a");

    double dis_B, dis_G, dis_R;
    double distance, max;
    dis_B = (ques[0] - player[0]) * (ques[0] - player[0]);
    dis_G = (ques[1] - player[1]) * (ques[1] - player[1]);
    dis_R = (ques[2] - player[2]) * (ques[2] - player[2]);
    distance = sqrt(dis_B + dis_G + dis_R);
    max = sqrt((255 * 255) + (255 * 255) + (255 * 255));
    point = 100 - (distance * 100 / max);
    qDebug("distance_BGR = %f\n", distance);
    qDebug("max_BGR = %f\n", max);
    qDebug("ques_BGR = %f,%f,%f\n", ques[0], ques[1], ques[2]);
    qDebug("player_BGR = %f,%f,%f\n\n", player[0], player[1], player[2]);

    //fprintf(fp,"theme color = %lf %lf %lf, player colors = %lf %lf %lf\n resutl = %lf\n",ques[0],ques[1],ques[2],player[0],player[1],player[2],point);
    //fclose(fp);
#endif
    return point;
}

double MainWindow::score_calc_Lch(const cv::Mat &ques_rgb, const cv::Mat &player_rgb)
{
    //FILE *fp;
    //fp = fopen("/home/user/Documents/QtResults/result_Lch","a");

    cv::Mat ques_lab(ques_rgb.size(), ques_rgb.type(), cv::Scalar(0, 0, 0));
    cv::Mat player_lab(player_rgb.size(), player_rgb.type(), cv::Scalar(0, 0, 0));
    // Labに変換
    cv::cvtColor(ques_rgb, ques_lab, cv::COLOR_BGR2Lab);
    cv::cvtColor(player_rgb, player_lab, cv::COLOR_BGR2Lab);
    // Lchに変換
    double ques_Lch[3], player_Lch[3];
    double delta_e94, e94_l, e94_c, e94_h;
    double kl, kc, kh, sl, sc, sh;
    double k1, k2;
    kc = kh = 1;
    kl = 1;
    if (kl == 1)
    {
        k1 = 0.045;
        k2 = 0.015;
    }
    else
    {
        k1 = 0.048;
        k2 = 0.014;
    }
    int y = 0, x = 0;

    // questionのLch
    ques_Lch[0] = ques_lab.at<cv::Vec3b>(y, x)[0];
    ques_Lch[1] = sqrt((ques_lab.at<cv::Vec3b>(y, x)[1] * ques_lab.at<cv::Vec3b>(y, x)[1]) + (ques_lab.at<cv::Vec3b>(y, x)[2] * ques_lab.at<cv::Vec3b>(y, x)[2]));
    ques_Lch[2] = atan2(ques_lab.at<cv::Vec3b>(y, x)[2], ques_lab.at<cv::Vec3b>(y, x)[1]);
    // playerのLch
    player_Lch[0] = player_lab.at<cv::Vec3b>(y, x)[0];
    player_Lch[1] = sqrt((player_lab.at<cv::Vec3b>(y, x)[1] * player_lab.at<cv::Vec3b>(y, x)[1]) + (player_lab.at<cv::Vec3b>(y, x)[2] * player_lab.at<cv::Vec3b>(y, x)[2]));
    player_Lch[2] = atan2(player_lab.at<cv::Vec3b>(y, x)[2], player_lab.at<cv::Vec3b>(y, x)[1]);
    sl = 1;
    sc = 1 + (k1 * ques_Lch[1]);
    sh = 1 + (k2 * ques_Lch[1]);
    e94_l = (ques_Lch[0] - player_Lch[0]) / (kl * sl);
    e94_c = (ques_Lch[1] - player_Lch[1]) / (kc * sc);
    e94_h = (ques_Lch[2] - player_Lch[2]) / (kh * sh);
    delta_e94 = sqrt((e94_l) * (e94_l) + (e94_c) * (e94_c) + (e94_h) * (e94_h));
    printf("delta_e94 = %lf\n", delta_e94);
    double point = 100 - ((delta_e94 / 225.001579) * 100);
    printf("\n");
    //fprintf(fp,"theme color = %d %d %d, player colors = %d %d %d\n resutl = %lf\n",ques_rgb.at<cv::Vec3b>(0,0)[0],ques_rgb.at<cv::Vec3b>(0,0)[1],ques_rgb.at<cv::Vec3b>(0,0)[2],player_rgb.at<cv::Vec3b>(0,0)[0],player_rgb.at<cv::Vec3b>(0,0)[1],player_rgb.at<cv::Vec3b>(0,0)[2],point);
    //fclose(fp);
    return point;
}

void MainWindow::ok2Result()
{
    buttonNext();
    double score_HSV;
    double score_BGR;
    double score_Lch;
    std::ostringstream str;
    std::ostringstream str2;
    std::ostringstream str3;
    score_HSV = score_calc(themeColor,player_col);
    str << score_HSV;
    score_BGR = score_calc_BGR(themeColor,player_col);
    str2 << score_BGR;
    score_Lch = score_calc_Lch(themeColor,player_col);
    str3 << score_Lch;
    //ui->getPoint->setText(QString::fromLocal8Bit(str.str());
    //ui->message->setText(QString::fromLocal8Bit(str2.str());
    ui->getPoint->setText(QString::fromLocal8Bit(str3.str()));

#ifdef sub
    ui->bgr->setText(QString::fromLocal8Bit("BGR 差の絶対値"));
    ui->hsv->setText(QString::fromLocal8Bit("hsv 差の絶対値"));
#elif dis
    ui->bgr->setText(QString::fromLocal8Bit("BGR 色の距離"));
    ui->hsv->setText(QString::fromLocal8Bit("hsv 色の距離"));
#else
    ui->hsv->setText(QString::fromLocal8Bit("Lch "));
#endif
    ui->yours->setText(QString::fromLocal8Bit("あなたが選んだ色はこちらです"));
    //ui->quesName->setText(QString::fromLocal8Bit("〇〇色"));
    cv::Mat resizeT(1,1,CV_8UC3);
    cv::Mat resizeP(1,1,CV_8UC3);
    cv::resize(themeColor,resizeT,cv::Size(),300,300);
    cv::cvtColor(resizeT,resizeT,cv::COLOR_BGR2RGB);
    QImage imgT(resizeT.data, resizeT.cols, resizeT.rows, QImage::Format_RGB888);
    cv::resize(player_col,resizeP,cv::Size(),300,300);
    cv::cvtColor(resizeP,resizeP,cv::COLOR_BGR2RGB);
    QImage imgP(resizeP.data, resizeP.cols, resizeP.rows, QImage::Format_RGB888);
    //ui->theme->setPixmap(QPixmap::fromImage(imgT));
    ui->pSelected->setPixmap(QPixmap::fromImage(imgP));
}

void MainWindow::toStart()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::toReplay()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::easyFlg()
{
    modeFlg = 1;
    ui->messageGM->setText(QString::fromLocal8Bit("EASY"));
}

void MainWindow::hardFlg()
{
    modeFlg = 2;
    ui->messageGM->setText(QString::fromLocal8Bit("HARD"));
}

void MainWindow::next2TargetMode()
{
    srand(std::time(nullptr));
    int ran = rand();
    cv::Vec3b src;
    std::stringstream str;
    int val = 0;
    Opencv::random(val, src, ran);
    str = Opencv::jed(val);
    cv::Mat_<cv::Vec3b> mat(200, 400, cv::Vec3b(src[0], src[1], src[2]));
    themeColor.at<cv::Vec3b>(0,0) = mat.at<cv::Vec3b>(0,0);
    if(modeFlg == 1){
        Opencv::setData(val, mat);
        Opencv::waku(mat);
        cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);
        ui -> ImageTarget -> setPixmap(QPixmap::fromImage(QImage((unsigned char*) mat.data, mat.cols, mat.rows, QImage::Format_RGB888)));
        ui -> NameTarget -> setText(QString::fromStdString(str.str()));
        ui -> theme-> setPixmap(QPixmap::fromImage(QImage((unsigned char*) mat.data, mat.cols, mat.rows, QImage::Format_RGB888)));
        ui -> quesName -> setText(QString::fromStdString(str.str()));
    }
    else if(modeFlg ==2){
        Opencv::setData(val, mat);
        ran = rand();
        Opencv::shape(ran, str, mat);
        Opencv::waku(mat);
        cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);
        ui -> ImageTarget -> setPixmap(QPixmap::fromImage(QImage((unsigned char*) mat.data, mat.cols, mat.rows, QImage::Format_RGB888)));
        ui -> NameTarget -> setText(QString::fromStdString(str.str()));
        ui -> theme -> setPixmap(QPixmap::fromImage(QImage((unsigned char*) mat.data, mat.cols, mat.rows, QImage::Format_RGB888)));
        ui -> quesName -> setText(QString::fromStdString(str.str()));
    }
    buttonNext();
}






