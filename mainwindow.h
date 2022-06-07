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

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void openImage();
    void changeHorizon();
    void buttonNext();
    void buttonBack();
    void buttonRetake();
    void takePicture();
    void positionOfImage();

private:
    cv::Mat colorChange(cv::Mat img);
    char* mojiArray();
    void mousePressEvent(QMouseEvent *event);

private slots:
    void mojiHyouji();

private:
    Ui::MainWindow *ui;
    QCamera *camera;
};
#endif // MAINWINDOW_H
