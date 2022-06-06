#ifndef STRAGEIMAGE_H
#define STRAGEIMAGE_H

#include <opencv2/opencv.hpp>

QT_BEGIN_NAMESPACE
namespace SImage { class StrageImage; }
QT_END_NAMESPACE

class StrageImage
{
private :
    cv::Mat strage;

    public :
    void strageImage(cv::Mat img);
    cv::Mat returnImage();
};

#endif // STRAGEIMAGE_H
